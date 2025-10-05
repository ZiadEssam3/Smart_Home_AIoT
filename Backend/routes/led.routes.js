const express = require('express');
const router = express.Router();
const {
    createLED,
    addLEDState,
    getAllLEDs,
    getLEDById,
    getLatestLEDState,
    getAllLEDStates,
    updateLED,
    deleteLED
} = require('../controllers/led.controller');

// Create new LED
router.post('/', createLED);

// Add new LED state
router.post('/:id', addLEDState);

// Get all LEDs
router.get('/', getAllLEDs);

// Get LED by ID
router.get('/:id', getLEDById);

// Get latest LED state
router.get('/:id/latest', getLatestLEDState);

// Get all LED states
router.get('/:id/all', getAllLEDStates);

// Update LED
router.put('/:id', updateLED);

// Delete LED
router.delete('/:id', deleteLED);

module.exports = router;
