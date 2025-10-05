import React, { useState } from 'react';
import { controlActuator } from '../../services/actuatorServices'; // Import the service function
import './actuatorControl.css'; // Import CSS for styling

function ActuatorControl({ label = 'Actuator', actuatorId }) {
    const [status, setStatus] = useState('');
    const [actuatorData, setActuatorData] = useState(null); // Store the returned actuator data

    const handleControl = async (action) => {
        try {
            const data = await controlActuator(actuatorId, action); // Send the action command
            setActuatorData(data); // Save the updated actuator data
            setStatus(`${label} is now ${action}`); // Update the status message
        } catch (error) {
            setStatus(`Failed to control ${label}. Please try again.`);
        }
    };

    return (
        <div className="actuator-control-container">
            <h1>{label}</h1>
            {/* Buttons for controlling the actuator */}
            <button
                onClick={() => handleControl('OPEN')}
                className="actuator-button open-button"
            >
                OPEN
            </button>
            <button
                onClick={() => handleControl('CLOSE')}
                className="actuator-button close-button"
            >
                CLOSE
            </button>
            <p className="actuator-status">{status}</p>
        </div>
    );
}

export default ActuatorControl;