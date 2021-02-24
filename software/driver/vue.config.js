module.exports = {
  transpileDependencies: ["vuetify"],
  pluginOptions: {
    electronBuilder: {
      nodeIntegration: true,
      externals: ['serialport']
    }
  },

  configureWebpack: {
    externals: {
      serialport: "commonjs2 serialport"
    }
  }
};
