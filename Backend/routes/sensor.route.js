const express = require('express');
const router = express.Router();

const {
    getAllSensors,
    getSensorById,
    createSensor,
    addSensorValue,
    getAllSensorValues,
    getLatestSensorValue,
    updateSensor,
    deleteSensor
} = require('../controllers/sensor.controller');

router.get('/', getAllSensors);
router.get('/:id', getSensorById);
router.post('/', createSensor);
router.post('/:id', addSensorValue);
router.get('/:id/all', getAllSensorValues);
router.get('/:id/latest', getLatestSensorValue);
router.put('/:id', updateSensor);
router.delete('/:id', deleteSensor);

module.exports = router;
