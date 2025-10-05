import React, { useState, useEffect } from 'react';
import { RadialBarChart, RadialBar } from 'recharts';
import { getLatestSensorRead } from '../../services/sensorServices';
import './circularGauge.css'; // Import the CSS file

function SoilMoistureGauge({ sensorId, barColor, valueColor, barLabel }) {
    const [latestValue, setLatestValue] = useState(null);
    const [error, setError] = useState(null);
    const [loading, setLoading] = useState(true);

    useEffect(() => {
        const fetchLatestValue = async () => {
            setLoading(true);
            try {
                const data = await getLatestSensorRead(sensorId);
                setLatestValue(data.value); // Assuming the value is the soil moisture percentage
                setError(null);
            } catch (err) {
                setError('Failed to fetch the latest sensor value.');
            } finally {
                setLoading(false);
            }
        };

        fetchLatestValue();
    }, [sensorId]);

    const data = [
        { name: 'Remaining', value: 100, fill: '#A9A9A9' }, // Gray base layer
        { name: 'Moisture', value: latestValue || 0, fill: barColor || '#FF0000' }, // Bar color based on props
    ];

    return (
        <div className="soil-moisture-container">
            {loading ? (
                <p className="soil-moisture-loading">Loading...</p>
            ) : error ? (
                <p className="soil-moisture-error">{error}</p>
            ) : (
                <>
                    <RadialBarChart
                        width={300}
                        height={300}
                        cx="50%"
                        cy="50%"
                        innerRadius="80%"
                        outerRadius="80%"
                        barSize={20} // Thickness of the bar
                        data={data}
                        startAngle={90} // Start at the top
                        endAngle={-270} // Fill counter-clockwise
                    >
                        <RadialBar
                            dataKey="value"
                            stackId="1" // Stack bars on top of each other
                            cornerRadius={10} // Smooth edges for the filled portion
                        />
                    </RadialBarChart>

                    {/* Center text for percentage */}
                    <div
                        className="soil-moisture-percentage"
                        style={{ color: valueColor || '#00ff00' }} // Text color based on props
                    >
                        {`${latestValue || 0}%`}
                    </div>
                    {/* Label below the chart */}
                    <h2 className="soil-moisture-title">{barLabel || 'Moisture Value'}</h2>
                </>
            )}
        </div>
    );
}

export default SoilMoistureGauge;