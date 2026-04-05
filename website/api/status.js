export default async function handler(req, res) {
  const READ_KEY = process.env.TS_READ_KEY;
  const CHANNEL_ID = process.env.TS_CHANNEL;

  try {
    const response = await fetch(
      `https://api.thingspeak.com/channels/${CHANNEL_ID}/fields/1/last.json?api_key=${READ_KEY}`
    );
    const data = await response.json();
    res.status(200).json(data);
  } catch (error) {
    res.status(500).json({ error: "Failed to fetch status" });
  }
}