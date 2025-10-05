const Sensor = require('../models/sensorModel');

// Get all sensors
let getAllSensors = async (req, res) => {
    try {
        const sensors = await Sensor.find();
        res.json(sensors);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Get sensor by ID
let getSensorById = async (req, res) => {
    try {
        const sensorId = req.params.id.trim();
        const sensor = await Sensor.findById(sensorId);
        if (!sensor) {
            return res.status(404).json({ message: 'Sensor not found' });
        }
        res.json(sensor);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Create new sensor
let createSensor = async (req, res) => {
    const { sensorType, location, pin, value } = req.body;
    const newSensor = new Sensor({ sensorType, location, pin, value });
    try {
        const savedSensor = await newSensor.save();
        res.status(201).json(savedSensor);
    } catch (err) {
        res.status(400).json({ message: err.message });
    }
};

// Add new value to sensor
let addSensorValue = async (req, res) => {
    try {
        const sensor = await Sensor.findById(req.params.id);
        if (!sensor) {
            return res.status(404).json({ message: 'Sensor not found' });
        }
        sensor.values.push({
            value: req.body.value,
            timestamp: Date.now(),
        });
        await sensor.save();
        res.json(sensor);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Get all values of sensor
let getAllSensorValues = async (req, res) => {
    try {
        const sensor = await Sensor.findById(req.params.id);
        if (!sensor) {
            return res.status(404).json({ message: 'Sensor not found' });
        }
        res.json(sensor.values);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Get latest sensor value
let getLatestSensorValue = async (req, res) => {
    try {
        const sensor = await Sensor.findById(req.params.id);
        if (!sensor) {
            return res.status(404).json({ message: 'Sensor not found' });
        }
        const latestValue = sensor.values[sensor.values.length - 1];
        res.json(latestValue);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Update sensor
let updateSensor = async (req, res) => {
    try {
        const sensorId = req.params.id.trim();
        const { sensorType, location } = req.body;
        if (!sensorType || !location) {
            return res.status(400).json({ message: 'Missing required fields: sensorType or location' });
        }
        const updatedSensor = await Sensor.findByIdAndUpdate(
            sensorId,
            { sensorType, location },
            { new: true }
        );
        if (!updatedSensor) {
            return res.status(404).json({ message: 'Sensor not found' });
        }
        res.status(200).json(updatedSensor);
    } catch (err) {
        res.status(500).json({ message: 'Error updating sensor', error: err.message });
    }
};

// Delete sensor
let deleteSensor = async (req, res) => {
    try {
        const sensorId = req.params.id.trim();
        const deletedSensor = await Sensor.findByIdAndDelete(sensorId);
        if (!deletedSensor) {
            return res.status(404).json({ message: 'Sensor not found' });
        }
        res.status(200).json({
            message: 'Sensor deleted successfully',
            deletedSensor
        });
    } catch (err) {
        res.status(500).json({ message: 'Error deleting sensor', error: err.message });
    }
};

// Export all functions
module.exports = {
    getAllSensors,
    getSensorById,
    createSensor,
    addSensorValue,
    getAllSensorValues,
    getLatestSensorValue,
    updateSensor,
    deleteSensor
};
