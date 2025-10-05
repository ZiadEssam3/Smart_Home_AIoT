const Actuator = require('../models/actuatorModel');

// Create a new actuator
let createActuator = async (req, res) => {
    const { name, location, pin, action } = req.body;
    const newActuator = new Actuator({ name, location, pin, action });

    try {
        const savedActuator = await newActuator.save();
        res.status(201).json(savedActuator);
    } catch (err) {
        res.status(400).json({ message: err.message });
    }
};

// Add a new action to an actuator
let addActuatorAction = async (req, res) => {
    try {
        const actuator = await Actuator.findById(req.params.id);
        if (!actuator) {
            return res.status(404).json({ message: 'Actuator not found' });
        }

        actuator.actions.push({
            action: req.body.action,
            timestamp: Date.now(),
        });

        await actuator.save();
        res.json(actuator);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Get all actuators
let getAllActuators = async (req, res) => {
    try {
        const actuators = await Actuator.find();
        res.json(actuators);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Get actuator by ID
let getActuatorById = async (req, res) => {
    try {
        const actuatorId = req.params.id.trim();
        const actuator = await Actuator.findById(actuatorId);

        if (!actuator) {
            return res.status(404).json({ message: 'Actuator not found' });
        }

        res.json(actuator);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Get latest actuator action
let getLatestActuatorAction = async (req, res) => {
    try {
        const actuator = await Actuator.findById(req.params.id);

        if (!actuator) {
            return res.status(404).json({ message: 'Actuator not found' });
        }

        const latestValue = actuator.actions[actuator.actions.length - 1];
        res.json(latestValue);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Get all actions for an actuator
let getAllActuatorActions = async (req, res) => {
    try {
        const actuator = await Actuator.findById(req.params.id);

        if (!actuator) {
            return res.status(404).json({ message: 'Actuator not found' });
        }

        res.json(actuator.actions);
    } catch (err) {
        res.status(500).json({ message: err.message });
    }
};

// Update actuator
let updateActuator = async (req, res) => {
    try {
        const actuatorId = req.params.id.trim();
        const { name, location, pin } = req.body;

        if (!name || !location || !pin) {
            return res.status(400).json({ message: 'Missing required fields: name, location, or pin' });
        }

        const updatedActuator = await Actuator.findByIdAndUpdate(
            actuatorId,
            { name, location, pin },
            { new: true }
        );

        if (!updatedActuator) {
            return res.status(404).json({ message: 'Actuator not found' });
        }

        res.status(200).json(updatedActuator);
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: 'Error updating Actuator', error: err.message });
    }
};

// Delete actuator
let deleteActuator = async (req, res) => {
    try {
        const actuatorId = req.params.id.trim();
        const deletedActuator = await Actuator.findByIdAndDelete(actuatorId);

        if (!deletedActuator) {
            return res.status(404).json({ message: 'Actuator not found' });
        }

        res.status(200).json({
            message: 'Actuator deleted successfully',
            deletedActuator
        });
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: 'Error deleting Actuator', error: err.message });
    }
};

// Export all functions
module.exports = {
    createActuator,
    addActuatorAction,
    getAllActuators,
    getActuatorById,
    getLatestActuatorAction,
    getAllActuatorActions,
    updateActuator,
    deleteActuator
};
