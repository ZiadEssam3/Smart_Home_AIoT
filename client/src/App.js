import React from 'react';
import SensorScatterChart from './components/scatterChart/scatterChart';
import SoilMoistureGauge from './components/circularGauge/circularGauge';
import LEDControl from './components/ledControl/ledControl';
import RGBControl from './components/RGBControl/RGBControl';
import ActuatorControl from './components/actuatorControl/actuatorControl';
import WaterLevelGauge from './components/waterGauge/waterGauge';
import LatestValuesChart from './components/TH/TH';
import Sidebar from './components/Sidebar/Sidebar';
function App() {
  const sensorId = '6765a21127520115c5583f11';
  const lineColor = '#ff045f';
  const xAxisLabel = 'Time';
  const yAxisLabel = 'Soil Data';

  const barColor = '#4caf50';
  const valueColor = '#4caf50';
  const barLabel = 'Soil Moisture Level';

  return (
    <div className="page-container">
      <Sidebar />
      <div className="main-content">
        {/* Scatter Chart */}
        <div className='SH-title'>
          <h2>Soil Moisture Data</h2>
        </div>
        <div className="large-section">
          <SensorScatterChart
            sensorId={sensorId}
            lineColor={lineColor}
            xAxisLabel={xAxisLabel}
            yAxisLabel={yAxisLabel}
          />
          <SoilMoistureGauge
            sensorId={sensorId}
            barColor={barColor}
            valueColor={valueColor}
            barLabel={barLabel}
          />
        </div>
        <div className='SH-title'>
          <h2>Smart Light Control Panel</h2>
        </div>
        <div className="large-section-light">
          <div className='normal-light'>
            <div className='normal-light-content'>

              <LEDControl label="Garden" ledId="676599ec27520115c558361f" />
            </div>
            <div className='normal-light-content'>
              <LEDControl label="First Floor" ledId="6787a73846243beea39b83a6" />
            </div>
            <div className='normal-light-content'>
              <LEDControl label="Balkon" ledId="6787a8d446243beea39b83a8" />
            </div>
          </div>
          <div className='rgb-light'>
            <RGBControl ledId="6787adba5ec2d2f971c62770" />
          </div>
        </div>

        <div className="large-section-light">
          <div className='water-level-cont'>
            <h2 className='water-title'>Water Level</h2>
            <WaterLevelGauge sensorId="6776de1c221f3557a5145a3f" barLabel="Tank Water Level" />
          </div>
          <div className='actuator-cont'>
            <ActuatorControl label="Garage Door" actuatorId="676b15f5ef5632dcd4084dff" />
          </div>
        </div>

        <div className='SH-title'>
          <h2>Environment INFO</h2>
        </div>
        <div className="large-section-light">
          <div className='temp-hum'>
            <LatestValuesChart
              temperatureSensorId="67dc391f570803ac0902b47b"
              humiditySensorId="67dc351d570803ac0902725b"
            />
          </div>
        </div>
      </div>
    </div >
  );
}

export default App;





