import React, { useState, useEffect } from 'react';
import { RadialBarChart, RadialBar } from 'recharts';
import { getLatestSensorRead } from '../../services/sensorServices'; // Replace with your service function
import './TH.css'; // Replace with your CSS styling

function LatestValuesChart({ temperatureSensorId, humiditySensorId }) {
    const [latestTemperature, setLatestTemperature] = useState(null);
    const [latestHumidity, setLatestHumidity] = useState(null);
    const [error, setError] = useState(null);
    const [loading, setLoading] = useState(true);

    useEffect(() => {
        const fetchLatestValues = async () => {
            setLoading(true);
            try {
                // Fetch latest temperature and humidity values (single objects)
                const temperatureData = await getLatestSensorRead(temperatureSensorId);
                const humidityData = await getLatestSensorRead(humiditySensorId);

                // Extract and set the values
                setLatestTemperature(temperatureData?.value || 0); // Get "value" from temperatureData
                setLatestHumidity(humidityData?.value || 0); // Get "value" from humidityData

                setError(null);
            } catch (err) {
                console.error('Error fetching sensor data:', err);
                setError('Failed to fetch sensor data. Please try again.');
            } finally {
                setLoading(false);
            }
        };

        fetchLatestValues();
    }, [temperatureSensorId, humiditySensorId]);

    // Always return the specified colors for temperature and humidity
    const determineTemperatureColor = () => '#FFA500'; // Orange for temperature
    const determineHumidityColor = () => '#ADD8E6'; // Light blue for humidity

    // Data for radial bar charts
    const temperatureData = [
        { name: 'Base', value: 100, fill: '#E0E0E0' },
        { name: 'Temperature', value: latestTemperature || 0, fill: determineTemperatureColor() },
    ];

    const humidityData = [
        { name: 'Base', value: 100, fill: '#E0E0E0' },
        { name: 'Humidity', value: latestHumidity || 0, fill: determineHumidityColor() },
    ];

    return (
        <div className="latest-values-container">
            {loading ? (
                <p className="loading">Loading...</p>
            ) : error ? (
                <p className="error">{error}</p>
            ) : (
                <div className="gauges">
                    {/* Temperature Gauge */}
                    <div className="gauge">
                        <RadialBarChart
                            width={150}
                            height={150}
                            cx="50%"
                            cy="50%"
                            innerRadius="80%"
                            outerRadius="80%"
                            barSize={20}
                            data={temperatureData}
                            startAngle={90}
                            endAngle={-270}
                        >
                            <RadialBar dataKey="value" stackId="1" cornerRadius={10} />
                        </RadialBarChart>
                        <div className="center-text" style={{ color: determineTemperatureColor() }}>
                            {`${latestTemperature || 0}°C`}
                        </div>
                        <p className="label">Temperature</p>
                    </div>

                    {/* Humidity Gauge */}
                    <div className="gauge">
                        <RadialBarChart
                            width={150}
                            height={150}
                            cx="50%"
                            cy="50%"
                            innerRadius="80%"
                            outerRadius="80%"
                            barSize={20}
                            data={humidityData}
                            startAngle={90}
                            endAngle={-270}
                        >
                            <RadialBar dataKey="value" stackId="1" cornerRadius={10} />
                        </RadialBarChart>
                        <div className="center-text" style={{ color: determineHumidityColor() }}>
                            {`${latestHumidity || 0}%`}
                        </div>
                        <p className="label">Humidity</p>
                    </div>
                </div>
            )}
        </div>
    );
}

export default LatestValuesChart;