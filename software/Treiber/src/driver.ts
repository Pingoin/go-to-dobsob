import { angleCalc, coordinateCalc} from "astronomy-bundle/utils";
import { createTimeOfInterest} from "astronomy-bundle/time";
import { createLocation } from "astronomy-bundle/earth";
import * as net from "net";
const breite = 53 + 44 / 60 + 16.44 / 3600;
const länge = 14 + 2 / 60 + 40.92 / 3600;

export class Driver {
  private port: number;
  private server: net.Server;

  constructor(port: number) {
    this.port = port;
    this.server = new net.Server();
    this.server.listen(port, function () {
      console.log(
        `Server listening for connection requests on socket localhost:${port}`
      );
    });
    console.log();
    this.server.on("connection", function (socket) {
      console.log("A new connection has been established.");

      // Now that a TCP connection has been established, the server can send data to
      // the client by writing to its socket.
      //socket.write('Hello, client.');

      // The server can also receive data from the client by reading from its socket.
      socket.on("data", function (chunk) {
        let RA = (chunk.readUInt32LE(12) / 0x100000000) * 360;
        RA-=RA>=180?360:0;
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
          radiusVector,
        } = coordinateCalc.equatorialSpherical2topocentricHorizontal(
          coords,
          location,
          T
        );
        const result = {
          LENGTH: chunk.readInt16LE(0), //0,1
          TIME: chunk.readBigInt64LE(2), //2,3,4,5,6
          RA: RA,
          DEC: DEC,
          Az: angleCalc.deg2angle(azimuth),
          alt: angleCalc.deg2angle(altitude),
          rad: radiusVector,
        };
        console.log(result);
      });

      // When the client requests to end the TCP connection with the server, the server
      // ends the connection.
      socket.on("end", function () {
        console.log("Closing connection with the client");
      });

      // Don't forget to catch error, for your own sake.
      socket.on("error", function (err) {
        console.log(`Error: ${err}`);
      });
    });
  }
}
