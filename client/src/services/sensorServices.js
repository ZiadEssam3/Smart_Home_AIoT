import axios from 'axios';
/**
 * Function to fetch sensor data.
 * @param {string} sensorId - The ID of the sensor.
 * @returns {Promise} - The fetched all sensor data.
 */
export const getSensorHistory = async (sensorId) => {
    try {
        const response = await axios.get(`http://localhost:3000/sensors/${sensorId}/all`);
        return response.data;
    } catch (error) {
        console.error('Error fetching all sensor value:', error.response?.data || error.message);
        throw error;
    }
};
/**
 * Function to fetch sensor data.
 * @param {string} sensorId - The ID of the sensor.
 * @returns {Promise} - The fetched latest sensor read.
 */
export const getLatestSensorRead = async (sensorId) => {
    try {
        const response = await axios.get(`http://localhost:3000/sensors/${sensorId}/latest`);
        return response.data;
    } catch (error) {
        console.error('Error fetching latest sensor value:', error.response?.data || error.message);
        throw error;
    }
};





