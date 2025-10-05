const express = require('express');
const cors = require('cors');
require('dotenv').config();

const connectDB = require('./config/database/connection');

const sensorRoutes = require('./routes/sensor.route');
const ledRoutes = require('./routes/led.routes');
const RGBRoutes = require('./routes/RGB.route');
const actuatorRoutes = require('./routes/actuator.route');

const PORT = process.env.PORT || 3000;

const app = express();
app.use(cors());
app.use(express.json());
connectDB();


app.use('/sensors', sensorRoutes);
app.use('/leds', ledRoutes);
app.use('/RGB', RGBRoutes);
app.use('/actuators', actuatorRoutes);


app.listen(PORT, () => {
  console.log(`Server Running At PORT http://locahost:${PORT}`);
});
