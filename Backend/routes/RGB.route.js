const express = require('express');
const router = express.Router();
const {
    createRGBLED,
    addRGBValues,
    getAllRGBLEDs,
    getRGBLEDById,
    getLatestRGBValue,
    getAllRGBValues,
    updateRGBLED,
    deleteRGBLED
} = require('../controllers/RGB.controller');

// Create new RGB LED
router.post('/', createRGBLED);

// Add new RGB values
router.post('/:id', addRGBValues);

// Get all RGB LEDs
router.get('/', getAllRGBLEDs);

// Get RGB LED by ID
router.get('/:id', getRGBLEDById);

// Get latest RGB LED value
router.get('/:id/latest', getLatestRGBValue);

// Get all RGB LED values
router.get('/:id/all', getAllRGBValues);

// Update RGB LED
router.put('/:id', updateRGBLED);

// Delete RGB LED
router.delete('/:id', deleteRGBLED);

module.exports = router;
