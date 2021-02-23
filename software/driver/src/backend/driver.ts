import aa from "./astronomical-algorithms";
import * as net from "net";
const latitude = 53 + 44 / 60 + 16.44 / 3600;
const longitude = 14 + 2 / 60 + 40.92 / 3600;
export class Driver {
  private port: number;
  private server: net.Server;
  private socket: net.Socket | null = null;
  private stellariumWorker: NodeJS.Timeout | null = null;
  private istPosition = {
    alt: 0,
    azimuth: 0
  };
  private sollPosition = {
    dec: 0,
    ra: 0
  };
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
    this.stellariumWorker = setInterval(this.sendPosition.bind(this), 2000);
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

    if (this.stellariumWorker == null) {
      this.stellariumWorker = setInterval(this.sendPosition.bind(this), 2000);
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
    if (this.socket) this.socket.end(() => (this.socket = null));
    if (this.stellariumWorker != null) clearTimeout(this.stellariumWorker);
    this.stellariumWorker = null;

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

    this.istPosition.alt = altitude;
    this.istPosition.azimuth = azimuth;
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
