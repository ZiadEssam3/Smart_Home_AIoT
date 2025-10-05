import React, { useState, useEffect } from 'react';
import { RadialBarChart, RadialBar } from 'recharts';
import { getLatestSensorRead } from '../../services/sensorServices';
import styles from './waterGauge.css'; // Import CSS module

function WaterLevelGauge({ sensorId, barLabel }) {
    const [latestValue, setLatestValue] = useState(null);
    const [error, setError] = useState(null);
    const [loading, setLoading] = useState(true);

    useEffect(() => {
        const fetchLatestValue = async () => {
            setLoading(true);
            try {
                const data = await getLatestSensorRead(sensorId);
                setLatestValue(data.value); // Assuming "value" contains the water level in cm
                setError(null);
            } catch (err) {
                setError('Failed to fetch the latest sensor value.');
            } finally {
                setLoading(false);
            }
        };

        fetchLatestValue();
    }, [sensorId]);

    // Determine the color based on water level
    const determineBarColor = (value) => {
        if (value < 3) return '#f44336'; // Red for less than 3cm
        if (value <= 5) return '#FFEB3B'; // Yellow for between 3cm and 5cm
        return '#4CAF50'; // Green for greater than 6cm
    };

    const data = [
        { name: 'Remaining', value: 8, fill: '#A9A9A9' }, // Gray base layer
        { name: 'Water Level', value: latestValue || 0, fill: determineBarColor(latestValue || 0) }, // Dynamic color
    ];

    return (
        <div className={styles.container}>
            {loading ? (
                <p className={styles.loading}>Loading...</p>
            ) : error ? (
                <p className={styles.error}>{error}</p>
            ) : (
                <>
                    <div className='chartContainer'>
                        <RadialBarChart
                            width={300}
                            height={300}
                            cx="50%"
                            cy="50%"
                            innerRadius="80%"
                            outerRadius="80%"
                            barSize={20}
                            data={data}
                            startAngle={90}
                            endAngle={-270}
                        >
                            <RadialBar
                                dataKey="value"
                                stackId="1"
                                cornerRadius={10}
                            />
                        </RadialBarChart>

                        {/* Center value */}
                        <div
                            className='centerText'
                            style={{ color: determineBarColor(latestValue || 0) }}
                        >
                            {`${latestValue || 0} cm`}
                        </div>
                    </div>

                    {/* Label below the chart */}
                    <h2 className='title'>{barLabel || 'Water Level'}</h2>
                </>
            )}
        </div>
    );
}

export default WaterLevelGauge;