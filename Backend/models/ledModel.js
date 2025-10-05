const mongoose = require('mongoose');
const ledSchema = mongoose.Schema({
    location: { 
        type: String, 
        required: true 
    },   
    pin: { 
        type: String, 
        required: true 
    },
    states: [
        {
            state: { type: Boolean, required: true }, 
            timestamp: { type: Date, default: Date.now }
        }
    ]
});

const LED = mongoose.model('LED', ledSchema);
module.exports = LED;
