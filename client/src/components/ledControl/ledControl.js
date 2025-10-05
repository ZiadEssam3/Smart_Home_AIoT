import React, { useState } from 'react';
import { controlLED } from '../../services/ledServices'; // Adjust the path based on your file structure
import LEDButton from '../ledButton/ledButton'; // Import the LEDButton component
import './LEDControl.css'; // Import the CSS file

function LEDControl({ label, ledId }) { // Add ledId as a required prop
    const [status, setStatus] = useState('');
    const [ledData, setLedData] = useState(null); // Store the returned LED data

    const handleControl = async (state) => {
        try {
            const data = await controlLED(ledId, state); // Use the dynamic LED ID prop
            setLedData(data); // Save the updated LED data to state
            setStatus(`${label} is now ${state === 0 ? 'ON' : 'OFF'}`); // Use the dynamic label
        } catch (error) {
            setStatus(`Failed to control ${label}. Please try again.`);
        }
    };

    return (
        <div className="led-control-container">
            <h2>{label}</h2> {/* Use the dynamic label */}
            {/* Buttons for turning the LED ON or OFF */}
            <LEDButton
                state={0}
                handleClick={handleControl}
                label={`Turn ON`}
                color="#4caf50"
            />
            <LEDButton
                state={1}
                handleClick={handleControl}
                label={`Turn OFF`}
                color="#f44336"
            />
            <p className="led-status">{status}</p>
        </div>
    );
}

export default LEDControl;