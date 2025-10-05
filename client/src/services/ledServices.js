import axios from 'axios';

/**
 * Function to control the LED state.
 * @param {string} ledId - The ID of the LED to control.
 * @param {number} state - The desired state of the LED (0 for OFF, 1 for ON).
 * @returns {Promise} - The API response.
 */
export const controlLED = async (ledId, state) => {
    try {
        const response = await axios.post(`http://localhost:3000/leds/${ledId}`, { state });
        return response.data; // Returns the updated LED data
    } catch (error) {
        console.error('Error controlling LED:', error.response?.data || error.message);
        throw error; // Rethrow error for further handling
    }
};


/**
 * Function to send RGB values to the RGB LED API.
 * @param {string} ledId - The ID of the RGB LED.
 * @param {number} red - The red intensity (0-255).
 * @param {number} green - The green intensity (0-255).
 * @param {number} blue - The blue intensity (0-255).
 * @returns {Promise} - The updated RGB LED data.
 */
export const controlRGBLED = async (ledId, red, green, blue) => {
    try {
        const response = await axios.post(`http://localhost:3000/RGB/${ledId}`, { red, green, blue });
        return response.data;
    } catch (error) {
        console.error('Error controlling RGB LED:', error.response?.data || error.message);
        throw error;
    }
};