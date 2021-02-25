/** @format */

import aa from "./astronomical-algorithms";
import * as net from "net";
import serialPort from "serialport";
const latitude = 53 + 44 / 60 + 16.44 / 3600;
const longitude = 14 + 2 / 60 + 40.92 / 3600;
export class Driver {
  private port: number;
  private server: net.Server;
  private socket: net.Socket | null = null;
  private COMport: serialPort | null = null;
  private parser: serialPort.parsers.Delimiter | null = null;
  private istPosition = {
    alt: 0,
    azimuth: 0
  };
  private sollPosition = {
    dec: 0,
    ra: 0
  };

  public connectSerial(port: string) {
    this.COMport = new serialPort(port, {
      baudRate: 115200
    });

    this.parser = this.COMport.pipe(
      new serialPort.parsers.Delimiter({ delimiter: "\n" })
    );
    this.parser.on("data", this.interpretCOM.bind(this));
  }

  private interpretCOM(data: Buffer) {
    const msg = data.toString().split(";");
    const command = parseInt(msg[0], 16);
    switch (command) {
      case 0x01:
        console.log(msg);
        this.istPosition.azimuth = parseFloat(msg[1]);
        this.istPosition.alt = parseFloat(msg[2]);
        this.sendPosition();
        break;

      default:
        break;
    }
  }

  public async listSerialPorts() {
    return serialPort.list().then(ports => {
      const portsList: string[] = [];
      ports.forEach(port => {
        portsList.push(port.path);
      });
      return portsList;
    });
  }
  private forcePosition = false;

  constructor(port: number) {
    this.port = port;
    this.server = new net.Server();
    this.server.listen(port, function() {
      console.log(
        `Server listening for connection requests on socket localhost:${port}`
      );
    });
    console.log();
    setInterval(this.writeOnController.bind(this), 500);
    this.server.on("connection", this.connected.bind(this));
  }

  private connected(socket: net.Socket) {
    console.log("A new connection has been established.");

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
    socket.on("end", this.closeSocket.bind(this));

    // Don't forget to catch error, for your own sake.
    socket.on("error", this.closeSocket.bind(this));
  }
  private closeSocket(err?: Error) {
    if (this.socket != null)
      this.socket.end(() => {
        this.socket = null;
      });
    this.socket = null;
    if (err) {
      console.log(`Error: ${err}`);
    } else {
      console.log("Closing connection with the client");
    }
  }

  private writeOnController() {
    const jd = aa.julianday.getJulianDay(new Date()) || 0;
    const {
      azimuth,
      altitude
    } = aa.coordinates.transformEquatorialToHorizontal(
      jd,
      longitude,
      latitude,
      this.sollPosition.ra,
      this.sollPosition.dec
    );
  }

  private setSollByStellarium(chunk: Buffer): void {
    console.log(chunk);
    this.sollPosition.ra = (chunk.readUInt32LE(12) / 0x100000000) * 24;
    this.sollPosition.dec = (chunk.readInt32LE(16) / 0x40000000) * 90;
  }

  private sendPosition(): void {
    const jd = aa.julianday.getJulianDay(new Date()) || 0;
    const eq = aa.coordinates.transformHorizontalToEquatorial(
      jd,
      this.istPosition.alt,
      this.istPosition.azimuth,
      longitude,
      latitude
    );
    eq.rightAscension -= 0.5 / 3600;
    eq.rightAscension = Math.max(eq.rightAscension, 0);
    const RAraw = Math.round((eq.rightAscension / 24) * 0x100000000);
    const DECraw = Math.round((eq.declination / 90) * 0x40000000);
    console.log(eq);
    console.log(this.socket);
    if (this.socket != null) {
      const buffer = Buffer.alloc(24, 0);
      buffer.writeInt16LE(24, 0);
      buffer.writeUInt32LE(RAraw, 12);
      buffer.writeInt32LE(DECraw, 16);
      for (let index = 0; index < 10; index++) {
        this.socket.write(buffer);
      }
      console.log("send");
    }
  }
}
