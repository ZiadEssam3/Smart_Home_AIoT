const RGBLED = require('../models/RGBledModel');

// Create new RGB LED
let createRGBLED = async (req, res) => {
    const { location, pin } = req.body;
    const newRGBLED = new RGBLED({ location, pin });

    try {
        const savedRGBLED = await newRGBLED.save();
        res.status(201).json(savedRGBLED);
    } catch (err) {
        res.status(400).json({ message: err.message });
    }
};

// Add new RGB values to an RGB LED
let addRGBValues = async (req, res) => {
    try {
        const rgbled = await RGBLED.findById(req.params.id);
        if (!rgbled) {
            return res.status(404).json({ message: 'RGB LED not found' });
        }

        rgbled.values.push({
            red: req.body.red,
            green: req.body.green,
            blue: req.body.blue,
            timestamp: Date.now(),
        });

        await rgbled.save();
        res.json(rgbled);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Get all RGB LEDs
let getAllRGBLEDs = async (req, res) => {
    try {
        const rgbleds = await RGBLED.find();
        res.json(rgbleds);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Get RGB LED by ID
let getRGBLEDById = async (req, res) => {
    try {
        const rgbledId = req.params.id.trim();
        const rgbled = await RGBLED.findById(rgbledId);

        if (!rgbled) {
            return res.status(404).json({ message: 'RGB LED not found' });
        }

        res.json(rgbled);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Get latest RGB LED value
let getLatestRGBValue = async (req, res) => {
    try {
        const rgbled = await RGBLED.findById(req.params.id);

        if (!rgbled) {
            return res.status(404).json({ message: 'RGB LED not found' });
        }

        const latestValue = rgbled.values[rgbled.values.length - 1];
        res.json(latestValue);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Get all RGB LED values
let getAllRGBValues = async (req, res) => {
    try {
        const rgbled = await RGBLED.findById(req.params.id);

        if (!rgbled) {
            return res.status(404).json({ message: 'RGB LED not found' });
        }

        res.json(rgbled.values);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Update RGB LED info
let updateRGBLED = async (req, res) => {
    try {
        const rgbledId = req.params.id.trim();
        const { location, pin } = req.body;

        if (!location || !pin) {
            return res.status(400).json({ message: 'Missing required fields: location or pin' });
        }

        const updatedRGBLED = await RGBLED.findByIdAndUpdate(
            rgbledId,
            { location, pin },
            { new: true }
        );

        if (!updatedRGBLED) {
            return res.status(404).json({ message: 'RGB LED not found' });
        }

        res.status(200).json(updatedRGBLED);
    } catch (err) {
        res.status(500).json({ message: 'Error updating RGB LED', error: err.message });
    }
};

// Delete RGB LED
let deleteRGBLED = async (req, res) => {
    try {
        const rgbledId = req.params.id.trim();
        const deletedRGBLED = await RGBLED.findByIdAndDelete(rgbledId);

        if (!deletedRGBLED) {
            return res.status(404).json({ message: 'RGB LED not found' });
        }

        res.status(200).json({
            message: 'RGB LED deleted successfully',
            deletedRGBLED
        });
    } catch (err) {
        res.status(500).json({ message: 'Error deleting RGB LED', error: err.message });
    }
};

module.exports = {
    createRGBLED,
    addRGBValues,
    getAllRGBLEDs,
    getRGBLEDById,
    getLatestRGBValue,
    getAllRGBValues,
    updateRGBLED,
    deleteRGBLED
};
