#pragma once

#include <Arduino.h>

static const char INDEX_HTML[] PROGMEM = R"HTML(
<!doctype html>
<html lang='en'>
<head>
  <meta charset='utf-8'>
  <meta name='viewport' content='width=device-width,initial-scale=1'>
  <title>USB Charger Monitor</title>
  <link rel='icon' type='image/png' href='data:image/png;base64,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'>
  <style>
    :root{--bg:#0b1220;--panel:#121b2e;--panel2:#18243d;--txt:#e5eeff;--muted:#9fb3d1;--acc:#65d6ff;--ok:#56e39f;--warn:#ffd166}
    *{box-sizing:border-box} body{margin:0;font-family:Inter,Segoe UI,Arial,sans-serif;background:radial-gradient(circle at 20% 0%,#1e2c4d 0,var(--bg) 42%);color:var(--txt)}
    .wrap{max-width:980px;margin:0 auto;padding:24px}
    .head{display:flex;justify-content:space-between;align-items:center;gap:16px;flex-wrap:wrap;margin-bottom:18px}
    h1{margin:0;font-size:1.6rem}
    .sub{color:var(--muted);font-size:.95rem}
    .status{padding:8px 12px;border-radius:999px;background:#1e2b47;border:1px solid #2d4067;font-size:.9rem}
    .grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(260px,1fr));gap:16px}
    .card{background:linear-gradient(180deg,var(--panel),var(--panel2));padding:18px;border:1px solid #2a3b61;border-radius:16px;box-shadow:0 10px 30px rgba(0,0,0,.25)}
    .title{font-weight:700;color:var(--acc);margin-bottom:10px}
    .row{display:flex;justify-content:space-between;padding:8px 0;border-bottom:1px dashed #2f4269}
    .row:last-child{border-bottom:none}
    .label{color:var(--muted)} .val{font-variant-numeric:tabular-nums;font-weight:600}
    .footer{margin-top:16px;color:var(--muted);font-size:.86rem}
  </style>
</head>
<body>
  <div class='wrap'>
    <div class='head'>
      <div>
        <h1>USB Charger Power Monitor</h1>
        <div class='sub'>Live telemetry from dual INA219 sensors</div>
      </div>
      <div id='conn' class='status'>Connecting...</div>
    </div>

    <div class='grid'>
      <section class='card'>
        <div class='title'>USB Type-C Charger</div>
        <div class='row'><span class='label'>Voltage</span><span id='c1v' class='val'>--.- V</span></div>
        <div class='row'><span class='label'>Current</span><span id='c1a' class='val'>--.- A</span></div>
        <div class='row'><span class='label'>Power</span><span id='c1w' class='val'>--.- W</span></div>
      </section>
      <section class='card'>
        <div class='title'>USB Charger</div>
        <div class='row'><span class='label'>Voltage</span><span id='c2v' class='val'>--.- V</span></div>
        <div class='row'><span class='label'>Current</span><span id='c2a' class='val'>--.- A</span></div>
        <div class='row'><span class='label'>Power</span><span id='c2w' class='val'>--.- W</span></div>
      </section>
    </div>

    <div class='footer'>Updates via WebSocket • OTA available via ElegantOTA routes.</div>
  </div>

  <script>
    const conn = document.getElementById('conn');
    const $ = id => document.getElementById(id);
    const set = (id, val, unit) => $(id).textContent = `${Number(val).toFixed(2)} ${unit}`;

    function connect() {
      const ws = new WebSocket(`ws://${location.host}/ws`);
      ws.onopen = () => { conn.textContent = 'Live Connected'; conn.style.color = '#56e39f'; };
      ws.onclose = () => { conn.textContent = 'Disconnected, retrying...'; conn.style.color = '#ffd166'; setTimeout(connect, 1200); };
      ws.onerror = () => ws.close();
      ws.onmessage = (e) => {
        const d = JSON.parse(e.data);
        set('c1v', d.c1_v, 'V'); set('c1a', d.c1_a, 'A'); set('c1w', d.c1_w, 'W');
        set('c2v', d.c2_v, 'V'); set('c2a', d.c2_a, 'A'); set('c2w', d.c2_w, 'W');
      };
    }
    connect();
  </script>
</body>
</html>
)HTML";
