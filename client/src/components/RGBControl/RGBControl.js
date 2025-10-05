import React, { useState } from 'react';
import { HexColorPicker } from "react-colorful"; // Import the color picker library
import { controlRGBLED } from '../../services/ledServices'; // Import your service function
import './RGBControl.css';

function RGBControl({ ledId }) {
    const [color, setColor] = useState("#ffffff"); // Default color (white)
    const [status, setStatus] = useState('');

    const handleSubmit = async () => {
        try {
            // Convert HEX color to RGB
            const red = parseInt(color.slice(1, 3), 16);
            const green = parseInt(color.slice(3, 5), 16);
            const blue = parseInt(color.slice(5, 7), 16);

            // Send RGB values to the backend
            const data = await controlRGBLED(ledId, red, green, blue);
            setStatus(`RGB LED updated: R=${red}, G=${green}, B=${blue}`);
        } catch (error) {
            setStatus('Failed to update RGB LED. Please try again.');
        }
    };

    return (
        <div className="rgb-control-container">
            <h1>RGB Control</h1>
            {/* Color Wheel */}
            <HexColorPicker
                color={color}
                onChange={setColor}
                style={{ width: '350px' }} // Specify the width directly
            />

            <button onClick={handleSubmit} className="submit-button">
                Update RGB
            </button>
            <p className="status-message">{status}</p>
        </div>
    );
}

export default RGBControl;