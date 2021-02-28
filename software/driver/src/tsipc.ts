import { createIpcChannel, createTypesafeIpc } from "electron-typesafe-ipc";

//first, describe the ipc communication schema - channel names, their direction (main->rend / rend->main) and type of their params (void means no params)
const ipcSchema = {
  main: {
    //main -> rend communication
    serialPorts: createIpcChannel<string[]>({ msg: "IPC_SERIAL_PORTS" })
  },
  rend: {
    //rend -> main communication
    setPort: createIpcChannel<string>({ msg: "IPC_SET_SERIAL_PORT" })
  }
};

//then create the typesafe ipc object via library function
export const tsipc = createTypesafeIpc(ipcSchema);
