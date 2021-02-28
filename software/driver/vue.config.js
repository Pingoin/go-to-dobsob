module.exports = {
  transpileDependencies: ["vuetify"],
  pluginOptions: {
    electronBuilder: {
      nodeIntegration: true,
      externals: ["serialport", "bluetoothSerialPort"]
    }
  },

  configureWebpack: {
    externals: {
      serialport: "commonjs2 serialport",
      bluetoothSerialPort: "commonjs2 bluetooth-serial-port"
    }
  }
};
