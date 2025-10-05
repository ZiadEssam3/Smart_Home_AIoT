const mongoose = require('mongoose');
const sensorSchema = new mongoose.Schema({
    sensorType: { 
        type: String, 
        required: true 
    },
    location: { 
        type: String, 
        required: true 
    },
    pin: { 
        type: String, 
        required: true 
    },
    values: [
        {
            value: { type: Number, required: true },
            timestamp: { type: Date, default: Date.now }
        }
    ]
});

const Sensor = mongoose.model('Sensor', sensorSchema);

module.exports = Sensor;























/*// models/sensorModel.js
const mongoose = require('mongoose');

// Define the schema for sensor data (e.g., temperature, soil moisture)
const sensorSchema = mongoose.Schema({
  sensorType: { type: String, required: true }, // e.g., DHT11, MQ-5, soil moisture
  value: { type: Number, required: true },      // Sensor reading value
  timestamp: { type: Date, default: Date.now }, // Timestamp of the data
});

// Create a model from the schema
const Sensor = mongoose.model('Sensor', sensorSchema);

module.exports = Sensor;*/


