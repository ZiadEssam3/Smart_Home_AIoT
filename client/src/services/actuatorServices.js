import axios from 'axios';

/**
 * Function to send actuator actions ("OPEN" or "CLOSE").
 * @param {string} actuatorId - The ID of the actuator.
 * @param {string} action - The command to control the actuator ("OPEN" or "CLOSE").
 * @returns {Promise} - The updated actuator data.
 */
export const controlActuator = async (actuatorId, action) => {
    try {
        const response = await axios.post(`http://localhost:3000/actuators/${actuatorId}`, { action });
        return response.data;
    } catch (error) {
        console.error('Error controlling actuator:', error.response?.data || error.message);
        throw error;
    }
};