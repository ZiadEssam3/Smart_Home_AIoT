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
    values: [
        {
            red: { type: Number, required: true, min: 0, max: 255 }, 
            green: { type: Number, required: true, min: 0, max: 255 }, 
            blue: { type: Number, required: true, min: 0, max: 255 }, 
            timestamp: { type: Date, default: Date.now }
        }
    ]
});

const RGBLED = mongoose.model('RGBLED', ledSchema);
module.exports = RGBLED;
