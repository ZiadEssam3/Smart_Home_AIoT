import React, { useState, useEffect } from 'react';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend } from 'recharts';
import './scatterChart.css';
import { getSensorHistory } from '../../services/sensorServices';

function SensorChart({ sensorId, lineColor, xAxisLabel, yAxisLabel }) {
    const [sensorData, setSensorData] = useState([]);
    const [error, setError] = useState(null);
    const [loading, setLoading] = useState(true);

    useEffect(() => {
        const fetchSensorHistory = async () => {
            setLoading(true);
            try {
                const data = await getSensorHistory(sensorId);
                const formattedData = data.map((item) => ({
                    timestamp: new Date(item.timestamp).toLocaleTimeString('en-US', {
                        hour: '2-digit',
                        minute: '2-digit',
                        hour12: true,
                    }),
                    value: item.value,
                }));
                setSensorData(formattedData);
                setError(null);
            } catch (err) {
                setError('Failed to fetch sensor history.');
            } finally {
                setLoading(false);
            }
        };

        fetchSensorHistory();
    }, [sensorId]);

    return (
        <div className="sensor-chart-container">
            {loading ? (
                <div className="sensor-chart-loading">
                    <p>Loading data...</p>
                </div>
            ) : error ? (
                <div className="sensor-chart-error">
                    <p>{error}</p>
                </div>
            ) : (
                <div className="sensor-chart-content">
                    <LineChart
                        width={700}
                        height={400}
                        data={sensorData}
                        margin={{ top: 10, right: 30, left: 30, bottom: 10 }}
                    >
                        <CartesianGrid strokeDasharray="3 3" />
                        <XAxis
                            dataKey="timestamp"
                            label={{ value: xAxisLabel, position: 'insideBottom', offset: 10 }} // Dynamic X-axis label
                            tick={false}
                        />
                        <YAxis
                            dataKey="value"
                            domain={[0, 1]}
                            label={{ value: yAxisLabel, angle: -90, position: 'insideLeft' }} // Dynamic Y-axis label
                        />
                        <Tooltip />
                        <Legend />
                        <Line
                            type="cardinal"
                            dataKey="value"
                            stroke={lineColor} // Line color is dynamic
                        />
                    </LineChart>
                </div>
            )}
        </div>
    );
}

export default SensorChart;