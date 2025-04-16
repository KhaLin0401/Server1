import express from 'express';
import mongoose from 'mongoose';
import dotenv from 'dotenv';

dotenv.config();

const app = express();
const PORT = process.env.PORT || 3000;
const MONGODB_URI = process.env.MONGODB_URI;

app.use(express.json()); // Đọc JSON từ body

// Kết nối MongoDB
mongoose.connect(MONGODB_URI)
  .then(() => console.log('✅ Đã kết nối MongoDB'))
  .catch(err => {
    console.error('❌ Lỗi kết nối MongoDB:', err);
    process.exit(1);
  });

// Tạo dynamic model dựa trên sensor_id
function getSensorModel(sensorId) {
  if (mongoose.models[sensorId]) {
    return mongoose.models[sensorId]; // Dùng lại model nếu đã tồn tại
  }

  const schema = new mongoose.Schema({}, { strict: false });
  return mongoose.model(sensorId, schema, sensorId); // sensorId = tên collection
}

// Route POST: ESP32 gửi dữ liệu
app.post('/stream_data', async (req, res) => {
  const data = req.body;
  console.log('📨 Payload nhận được:', req.body);
  const { sensor_id } = data;

  if (!sensor_id) {
    return res.status(400).send('Thiếu sensor_id');
  }

  try {
    const SensorModel = getSensorModel(sensor_id);
    const entry = new SensorModel({ ...data, timestamp: new Date() });
    await entry.save();
    console.log(`📥 Dữ liệu từ ${sensor_id}:`, data);
    res.status(200).send('Đã lưu thành công');
  } catch (err) {
    console.error('❌ Lỗi ghi dữ liệu:', err);
    res.status(500).send('Lỗi server');
  }
});

// Route GET: frontend lấy dữ liệu theo sensor_id
app.get('/sensor_data/:sensor_id', async (req, res) => {
  const { sensor_id } = req.params;
  const limit = parseInt(req.query.limit) || 100;

  try {
    const SensorModel = getSensorModel(sensor_id);
    const data = await SensorModel.find().sort({ timestamp: -1 }).limit(limit);
    res.json(data);
  } catch (err) {
    console.error('❌ Lỗi truy xuất:', err);
    res.status(500).send('Không lấy được dữ liệu');
  }
});

// Khởi động server
app.listen(PORT, () => {
  console.log(`🚀 Server đang chạy tại http://localhost:${PORT}`);
});
