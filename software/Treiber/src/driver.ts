import { angleCalc, coordinateCalc } from "astronomy-bundle/utils";
import { createTimeOfInterest } from "astronomy-bundle/time";
import { createLocation } from "astronomy-bundle/earth";
import * as net from "net";
import { copyFileSync } from "fs";
const breite = 53 + 44 / 60 + 16.44 / 3600;
const länge = 14 + 2 / 60 + 40.92 / 3600;

function rad2deg(rad: number): number {
  return (rad / Math.PI) * 180;
}

function deg2rad(deg: number) {
  return (deg / 180) * Math.PI;
}
export class Driver {
  private port: number;
  private server: net.Server;
  private socket: net.Socket | null = null;
  private istPosition = {
    alt: 0,
    azimuth: 0,
  };
  private sollPosition = {
    alt: 0,
    azimuth: 0,
  };
  private postitionEq = {
    rightAscension: 0,
    declination: 0,
    radiusVector: 0,
  };
  constructor(port: number) {
    this.port = port;
    this.server = new net.Server();
    this.server.listen(port, function () {
      console.log(
        `Server listening for connection requests on socket localhost:${port}`
      );
    });
    console.log();
    this.server.on("connection", this.connected.bind(this));
  }

  private connected(socket: net.Socket) {
    console.log("A new connection has been established.");
    setInterval(this.sendPosition.bind(this), 1000, socket);
    // Now that a TCP connection has been established, the server can send data to
    // the client by writing to its socket.
    //socket.write('Hello, client.');
    if (this.socket == null) {
      this.socket = socket;
    }

    // The server can also receive data from the client by reading from its socket.
    socket.on("data", this.setSollByStellarium.bind(this));

    // When the client requests to end the TCP connection with the server, the server
    // ends the connection.
    socket.on("end", function () {
      this.socket = null;
      console.log("Closing connection with the client");
    });

    // Don't forget to catch error, for your own sake.
    socket.on("error", function (err) {
      console.log(`Error: ${err}`);
    });
  }

  private setSollByStellarium(chunk: Buffer): void {
    console.log(chunk);
    let RA = (chunk.readUInt32LE(12) / 0x100000000) * 360;
    RA -= RA >= 180 ? 360 : 0;
    const DEC = (chunk.readInt32LE(16) / 0x40000000) * 90;
    const toi = createTimeOfInterest.fromCurrentTime();
    const location = createLocation(breite, länge);
    const coords = {
      rightAscension: RA,
      declination: DEC,
      radiusVector: 0,
    };
    const T = toi.getJulianCenturiesJ2000();
    const {
      azimuth,
      altitude,
    } = coordinateCalc.equatorialSpherical2topocentricHorizontal(
      coords,
      location,
      T
    );
    this.sollPosition.azimuth = azimuth;
    this.sollPosition.alt = altitude;
    const result = {
      LENGTH: chunk.readInt16LE(0), //0,1
      TIME: chunk.readBigInt64LE(2), //2,3,4,5,6
      RA: RA,
      DEC: DEC,
      Az: angleCalc.deg2angle(this.sollPosition.azimuth),
      alt: angleCalc.deg2angle(this.sollPosition.alt),
    };
    console.log(result);
    if (this.socket != null) {
      const buffer = Buffer.alloc(24, chunk);
      buffer.writeInt32BE(0, 20);
      buffer.writeInt16LE(24, 0);
      this.socket.write(buffer);
      console.log(chunk);
      console.log(buffer);
    }
    this.istPosition = this.sollPosition;
  }

  private sendPosition(socket: net.Socket): void {
    const toi = createTimeOfInterest.fromCurrentTime();
    const T = toi.getJulianCenturiesJ2000();
    console.log(this.istPosition.alt);
    const DEC = Math.asin(
      Math.sin(deg2rad(breite)) * Math.sin(deg2rad(this.istPosition.alt)) -
        Math.cos(deg2rad(breite)) *
          Math.cos(deg2rad(this.istPosition.azimuth)) *
          Math.cos(deg2rad(this.istPosition.alt))
    );
    const stundenwinkel = Math.atan2(
      Math.cos(deg2rad(this.istPosition.azimuth)),
      Math.sin(deg2rad(breite)) * Math.cos(deg2rad(this.istPosition.azimuth)) +
        Math.cos(deg2rad(breite)) * Math.tan(deg2rad(this.istPosition.alt))
    );
    console.log(angleCalc.deg2angle(DEC));
    console.log(angleCalc.deg2angle(stundenwinkel));

  }
}
