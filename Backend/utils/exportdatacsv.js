const { MongoClient } = require('mongodb');
const fs = require('fs');
const { parse } = require('json2csv');
const path = require('path'); 

const uri = 'mongodb://localhost:27017';
const dbName = 'smartHome';

async function exportCollections() {
  const client = new MongoClient(uri);
  const allDocuments = []; 

  try {
    await client.connect();
    console.log('Connected to MongoDB');
    const db = client.db(dbName);
    const collections = await db.listCollections().toArray();

    for (const collection of collections) {
      const collectionName = collection.name;
      const coll = db.collection(collectionName);
      const documents = await coll.find({}).toArray();

      if (documents.length > 0) {
        const filteredDocuments = documents.map(({ _id, sensorType, location, pin, values, __v, name, actions, states }) => ({
          _id,
          sensorType,
          location,
          pin,
          values: values.map(v => v.value).join(','), 
          __v,
          collectionName, 
          name,
          actions,
          states
        }));
        allDocuments.push(...filteredDocuments);
        console.log(`Collected ${filteredDocuments.length} documents from ${collectionName}`);
      }
    }

    if (allDocuments.length > 0) {
      const csvFields = ["_id", "sensorType", "location", "pin", "values", "__v", "collectionName", "name", "actions", "states"];
      const csvData = parse(allDocuments, { fields: csvFields });
      const folderPath = path.join(__dirname, 'data');
      if (!fs.existsSync(folderPath)) {
        fs.mkdirSync(folderPath);
        console.log('Created "data" folder');
      }
      const filePath = path.join(folderPath, 'filtered_collections.csv');
      fs.writeFileSync(filePath, csvData);
      console.log(`Exported filtered collections to ${filePath}`);
    } else {
      console.log('No data found to export.');
    }
  } catch (err) {
    console.error('Error exporting data:', err);
  } finally {
    await client.close();
    console.log('Database connection closed');
  }
}

exportCollections();