const mongoose = require('mongoose');
require('dotenv').config();
const connectDB = async () => {
    try {
        const conn = await mongoose.connect(process.env.DB_URI, {
            useNewUrlParser: true,
            useUnifiedTopology: true,
        });
        console.log('Database Connected Sucessfully.....');
    } catch (err) {
        console.error(err);
        process.exit(1);
    }
};

module.exports = connectDB;

