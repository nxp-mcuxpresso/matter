// Standard webpack configuration file for MatterApplicationUIMaterial

const path = require('path');

module.exports = {
  entry: './src/index.js',
  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: 'matter-application-ui-material.js',
    library: 'MatterApplicationUIMaterial',

    // This will expose the library to the global scope
    libraryTarget: 'umd',
  },
  module: {
    rules: [
      {
        test: /\.js$/,
        exclude: /node_modules/,
        use: {
          loader: 'babel-loader',
        },
      },
    ],
  },

  // This will allow the library to be used in a browser environment

  externals: {
    react: 'react',
    'react-dom': 'react-dom',
    '@material-ui/core': '@material-ui/core',
    '@material-ui/icons': '@material-ui/icons',
    '@material-ui/lab': '@material-ui/lab',
  },
};