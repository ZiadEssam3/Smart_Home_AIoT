import React from 'react';

function LEDButton({ state, handleClick, label, color }) {
    return (
        <button
            onClick={() => handleClick(state)}
            className="led-button"
            style={{ backgroundColor: color }}
        >
            {label}
        </button>
    );
}

export default LEDButton;