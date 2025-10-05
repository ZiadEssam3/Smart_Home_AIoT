const express = require('express');
const router = express.Router();
const {
    createActuator,
    addActuatorAction,
    getAllActuators,
    getActuatorById,
    getLatestActuatorAction,
    getAllActuatorActions,
    updateActuator,
    deleteActuator
} = require('../controllers/actuator.controller');

// Routes mapping
router.post('/', createActuator);
router.post('/:id', addActuatorAction);
router.get('/', getAllActuators);
router.get('/:id', getActuatorById);
router.get('/:id/latest', getLatestActuatorAction);
router.get('/:id/all', getAllActuatorActions);
router.put('/:id', updateActuator);
router.delete('/:id', deleteActuator);

module.exports = router;
