const mongoose = require('mongoose');
const actuatorSchema = mongoose.Schema({
    name: { 
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
    actions: [
        {
            action: { type: String, required: true }, 
            timestamp: { type: Date, default: Date.now }, 
        }
    ]
});

const Actuator = mongoose.model('Actuator', actuatorSchema);
module.exports = Actuator;
