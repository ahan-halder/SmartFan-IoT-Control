export default async function handler(req, res) {
  const { speed } = req.query;
  const WRITE_KEY = process.env.TS_WRITE_KEY;

  if (speed === undefined) {
    return res.status(400).json({ error: "Speed parameter is missing" });
  }

  try {
    const response = await fetch(
      `https://api.thingspeak.com/update?api_key=${WRITE_KEY}&field3=${speed}`
    );
    const result = await response.text();
    
    res.status(200).send(result);
  } catch (error) {
    res.status(500).json({ error: "External API error" });
  }
}