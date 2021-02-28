import { tsipc } from "../tsipc";

export default class SocketHelper {
  arsch = "Sackratte";
  constructor() {
    tsipc.rend.on.serialPorts(ports => {
      this.ports = ports;
    });
  }

  ports: string[] = [];
  setPort(val: string) {
    tsipc.rend.send.setPort(val);
  }
}
