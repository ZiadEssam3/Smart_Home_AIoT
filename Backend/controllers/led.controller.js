const LED = require('../models/ledModel');

// Create new LED
let createLED = async (req, res) => {
    const { location, pin, state } = req.body;
    const newLED = new LED({ location, pin, state });

    try {
        const savedLED = await newLED.save();
        res.status(201).json(savedLED);
    } catch (err) {
        res.status(400).json({ message: err.message });
    }
};

// Add new LED state
let addLEDState = async (req, res) => {
    try {
        const led = await LED.findById(req.params.id);
        if (!led) {
            return res.status(404).json({ message: 'LED not found' });
        }

        led.states.push({
            state: req.body.state,
            timestamp: Date.now(),
        });

        await led.save();
        res.json(led);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Get all LEDs
let getAllLEDs = async (req, res) => {
    try {
        const leds = await LED.find();
        res.json(leds);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Get LED by ID
let getLEDById = async (req, res) => {
    try {
        const ledId = req.params.id.trim();
        const led = await LED.findById(ledId);

        if (!led) {
            return res.status(404).json({ message: 'LED not found' });
        }

        res.json(led);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Get latest LED state
let getLatestLEDState = async (req, res) => {
    try {
        const led = await LED.findById(req.params.id);

        if (!led) {
            return res.status(404).json({ message: 'LED not found' });
        }

        const latestValue = led.states[led.states.length - 1];
        res.json(latestValue);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Get all LED states
let getAllLEDStates = async (req, res) => {
    try {
        const led = await LED.findById(req.params.id);

        if (!led) {
            return res.status(404).json({ message: 'LED not found' });
        }

        res.json(led.states);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Update LED
let updateLED = async (req, res) => {
    try {
        const ledId = req.params.id.trim();
        const { location, pin } = req.body;

        if (!location || !pin) {
            return res.status(400).json({ message: 'Missing required fields: location or pin' });
        }

        const updatedLED = await LED.findByIdAndUpdate(
            ledId,
            { location, pin },
            { new: true }
        );

        if (!updatedLED) {
            return res.status(404).json({ message: 'LED not found' });
        }

        res.status(200).json(updatedLED);
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: 'Error updating LED', error: err.message });
    }
};

// Delete LED
let deleteLED = async (req, res) => {
    try {
        const ledId = req.params.id.trim();
        const deletedLED = await LED.findByIdAndDelete(ledId);

        if (!deletedLED) {
            return res.status(404).json({ message: 'LED not found' });
        }

        res.status(200).json({
            message: 'LED deleted successfully',
            deletedLED
        });
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: 'Error deleting LED', error: err.message });
    }
};

module.exports = {
    createLED,
    addLEDState,
    getAllLEDs,
    getLEDById,
    getLatestLEDState,
    getAllLEDStates,
    updateLED,
    deleteLED
};
