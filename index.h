#pragma once
#include <pgmspace.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no" />
  <title>University Shop Locker</title>
  <style>
    /* --- BATSTATEU THEME (Strict Red & White) --- */
    :root {
      --primary: #991B1B;       /* Batangas Red */
      --primary-hover: #7F1D1D;
      --primary-light: #FEF2F2; 
      --white: #FFFFFF;
      --bg-app: #F9FAFB;
      --text-dark: #111827;
      --text-gray: #6B7280;
      --border: #E5E7EB;
      --shadow: 0 4px 6px -1px rgba(0, 0, 0, 0.05), 0 2px 4px -1px rgba(0, 0, 0, 0.03);
      --radius: 12px;
    }

    body {
      margin: 0;
      font-family: 'Segoe UI', Roboto, Helvetica, Arial, sans-serif;
      background: var(--bg-app);
      color: var(--text-dark);
      display: flex;
      flex-direction: column;
      height: 100vh;
      -webkit-font-smoothing: antialiased;
    }

    /* --- BRANDED HEADER --- */
    header {
      background: var(--white);
      padding: 12px 0; /* Tighter padding for smaller logo */
      text-align: center;
      border-bottom: 1px solid var(--border);
      box-shadow: var(--shadow);
      z-index: 10;
    }
    
    /* LOGO STYLING - 35px */
    .bsu-logo {
      height: 35px; 
      width: auto; 
      display: block; 
      margin: 0 auto 6px auto; /* Tighter margin */
    }

    .brand-sub {
      font-size: 10px;
      text-transform: uppercase;
      letter-spacing: 1.5px;
      color: var(--text-gray);
      font-weight: 600;
      margin-bottom: 2px;
    }
    .brand-main {
      font-size: 18px; /* Adjusted to match smaller logo */
      font-weight: 800;
      color: var(--primary);
      letter-spacing: -0.5px;
    }

    /* --- NAVIGATION --- */
    .tabs { display:none; background: var(--white); margin-top: 1px; }
    .tabs.visible { display:flex; }
    .tab { 
      flex:1; padding: 16px; text-align:center; cursor:pointer; 
      font-size: 13px; font-weight: 600; color: var(--text-gray); 
      border-bottom: 2px solid transparent; transition: 0.2s;
    }
    .tab.active { 
      color: var(--primary); border-bottom: 2px solid var(--primary); 
      background: linear-gradient(to bottom, #fff, #FFF5F5);
    }

    /* --- CONTAINERS --- */
    .view { display:none; flex:1; padding: 24px; overflow-y: auto; }
    .view.active { display: block; animation: fadeIn 0.3s ease-in-out; }
    .container { max-width: 380px; margin: 0 auto; }
    .hidden { display: none !important; }

    /* --- BUTTONS & INPUTS --- */
    .btn-main {
      width: 100%; padding: 16px; background: var(--primary); color: var(--white);
      font-weight: 600; border: none; border-radius: var(--radius); cursor: pointer;
      font-size: 15px; box-shadow: 0 4px 10px rgba(153, 27, 27, 0.2); transition: 0.1s;
    }
    .btn-main:active { transform: scale(0.98); background: var(--primary-hover); }
    
    .btn-outline {
      background: transparent; border: 2px solid var(--primary); color: var(--primary);
      margin-top: 12px; box-shadow: none;
    }

    .input-field {
      width: 100%; padding: 16px; margin: 8px 0 20px 0; background: var(--white);
      border: 1px solid var(--border); border-radius: var(--radius);
      box-sizing: border-box; text-align: center; font-size: 16px;
    }
    .input-field:focus { outline: none; border-color: var(--primary); }

    /* --- KEYPAD --- */
    .display-screen {
      background: var(--white); border: 1px solid var(--border); border-radius: var(--radius);
      padding: 24px; text-align: center; margin-bottom: 24px;
      box-shadow: inset 0 2px 4px rgba(0,0,0,0.02);
    }
    .pin-dots {
      font-family: monospace; font-size: 32px; letter-spacing: 12px;
      color: var(--text-dark); font-weight: 700;
    }
    .status-text { font-size: 12px; color: var(--text-gray); margin-bottom: 8px; text-transform: uppercase; font-weight: 600; }
    
    .keys { display: grid; grid-template-columns: repeat(3, 1fr); gap: 16px; justify-items: center; }
    .key {
      width: 65px; height: 65px; background: var(--white); border: 1px solid var(--border);
      border-radius: 50%; font-size: 22px; color: var(--text-dark); cursor: pointer;
      display: flex; align-items: center; justify-content: center;
      box-shadow: 0 2px 4px rgba(0,0,0,0.05); transition: 0.1s;
    }
    .key:active { background: var(--primary-light); color: var(--primary); border-color: var(--primary-light); }
    .key.action { font-size: 14px; font-weight: 700; background: transparent; border: none; box-shadow: none; color: var(--text-gray); }

    /* --- AVAILABILITY GRID (Red = Free, Gray = Busy) --- */
    .grid-label { font-size: 12px; color: var(--text-gray); margin-bottom: 10px; font-weight: 600; text-align: center; }
    .status-grid { 
      display: grid; grid-template-columns: repeat(5, 1fr); gap: 8px; 
      padding: 12px; background: var(--white); border-radius: var(--radius); 
      border: 1px solid var(--border); margin-bottom: 24px;
    }
    .dot {
      height: 25px; width: 100%; border-radius: 6px;
      display: flex; align-items: center; justify-content: center;
      font-size: 10px; font-weight: 700; transition: all 0.3s;
    }
    .dot.free { background: var(--primary); color: white; box-shadow: 0 2px 4px rgba(153, 27, 27, 0.3); }
    .dot.busy { background: var(--border); color: var(--text-gray); opacity: 0.5; }

    /* --- ADMIN CARDS --- */
    .admin-controls { display: flex; gap: 10px; margin-bottom: 20px; }
    .btn-small {
      flex: 1; padding: 10px; font-size: 12px; font-weight: 700;
      background: var(--white); border: 1px solid var(--border);
      border-radius: 8px; color: var(--text-gray); cursor: pointer;
    }
    .btn-small.active { background: var(--text-dark); color: white; border-color: var(--text-dark); }
    
    .grid-admin { display: grid; grid-template-columns: repeat(auto-fill, minmax(140px, 1fr)); gap: 16px; }
    .card {
      background: var(--white); border: 1px solid var(--border); border-radius: 16px;
      padding: 16px; display: flex; flex-direction: column; gap: 10px;
      box-shadow: var(--shadow); position: relative; overflow: hidden;
    }
    .card.unlocked { border-left: 4px solid var(--primary); }
    .card.locked { border-left: 4px solid var(--text-gray); }
    
    .card-top { display: flex; justify-content: space-between; font-size: 12px; font-weight: 700; color: var(--text-gray); }
    .pin-code { font-size: 20px; font-family: monospace; font-weight: 700; text-align: center; color: var(--text-dark); padding: 8px 0; letter-spacing: 2px; }
    
    .card-actions { display: grid; grid-template-columns: 1fr 1fr; gap: 6px; }
    .btn-act {
      padding: 8px; border-radius: 6px; border: 1px solid var(--border);
      background: #F9FAFB; font-size: 10px; font-weight: 700; color: var(--text-dark); cursor: pointer;
    }
    .btn-gen { width: 100%; background: var(--primary); color: white; border: none; }

    /* --- HISTORY TABLE --- */
    table { width: 100%; border-collapse: collapse; background: var(--white); border-radius: var(--radius); overflow: hidden; box-shadow: var(--shadow); }
    th { background: #F9FAFB; color: var(--text-gray); font-size: 11px; text-transform: uppercase; padding: 12px; text-align: left; border-bottom: 1px solid var(--border); }
    td { padding: 12px; font-size: 13px; border-bottom: 1px solid var(--border); color: var(--text-dark); }
    .badge { padding: 4px 8px; border-radius: 12px; font-size: 10px; font-weight: 700; }
    .badge.user { background: var(--text-dark); color: white; }
    .badge.admin { background: var(--primary-light); color: var(--primary); }

    /* --- TOAST --- */
    #toast {
      position: fixed; top: 20px; left: 50%; transform: translateX(-50%) translateY(-100px);
      background: var(--text-dark); color: white; padding: 12px 24px;
      border-radius: 30px; font-size: 14px; font-weight: 600;
      box-shadow: 0 10px 15px -3px rgba(0,0,0,0.1);
      transition: transform 0.4s cubic-bezier(0.175, 0.885, 0.32, 1.275);
      z-index: 100;
    }
    #toast.show { transform: translateX(-50%) translateY(0); }

    @keyframes fadeIn { from { opacity:0; transform:translateY(10px); } to { opacity:1; transform:translateY(0); } }
  </style>
</head>
<body>

  <header>
    <img src="data:image/jpeg;base64,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" class="bsu-logo" alt="BSU Logo" />
    
    <div class="brand-sub">Batangas State University</div>
    <div class="brand-main">University Shop</div>
  </header>

  <div class="tabs">
    <div class="tab" id="tab-cus" onclick="switchTab('customer')">LOCKER ACCESS</div>
    <div class="tab" id="tab-adm" onclick="checkAdminTab()">ADMIN PANEL</div>
    <div class="tab" onclick="goToPortal('home')">HOME</div>
  </div>

  <div id="home-portal" class="view active">
    <div class="container" style="text-align: center; padding-top: 40px;">
        <h2 style="font-size: 24px; color: var(--text-dark);">Welcome</h2>
        <p style="color: var(--text-gray); margin-bottom: 40px; line-height: 1.5;">
          Secure storage for bought items.<br>Select your portal to begin.
        </p>
        
        <button class="btn-main" onclick="goToPortal('customer')">CUSTOMER ACCESS</button>
        <button class="btn-main btn-outline" onclick="goToPortal('admin')">ADMINISTRATION</button>
        
        <p style="margin-top: 60px; font-size: 11px; color: #9CA3AF;">&copy; 2024 Batangas State University</p>
    </div>
  </div>

  <div id="customer" class="view">
    <div class="container">
      
      <div id="step-keypad">
        <div class="grid-label">LOCKER AVAILABILITY</div>
        <div class="status-grid" id="public-grid">
           </div>

        <div class="display-screen">
          <div class="status-text">ENTER ACCESS CODE</div>
          <div class="pin-dots" id="disp">----</div>
        </div>

        <div class="keys">
          <button class="key" onclick="num(1)">1</button><button class="key" onclick="num(2)">2</button><button class="key" onclick="num(3)">3</button>
          <button class="key" onclick="num(4)">4</button><button class="key" onclick="num(5)">5</button><button class="key" onclick="num(6)">6</button>
          <button class="key" onclick="num(7)">7</button><button class="key" onclick="num(8)">8</button><button class="key" onclick="num(9)">9</button>
          <button class="key action" onclick="clr()">CLEAR</button>
          <button class="key" onclick="num(0)">0</button>
          <button class="key action" style="color: var(--primary);" onclick="useCode()">ENTER</button>
        </div>
      </div>

      <div id="step-dash" class="hidden" style="text-align: center; padding-top: 40px;">
        <div style="font-size: 60px; margin-bottom: 20px;">🔓</div>
        <h2 style="margin-bottom: 10px;">Locker <span id="dash-id">--</span> Open</h2>
        <p style="color: var(--text-gray); margin-bottom: 40px;">
          Your session is active. Close the door securely before finishing.
        </p>
        <button class="btn-main" onclick="lockAndLeave()">LOCK & FINISH SESSION</button>
      </div>

    </div>
  </div>

  <div id="admin" class="view">
    <div class="container">
      
      <div id="login-screen">
        <h2 style="text-align:center;">Admin Login</h2>
        <p style="text-align:center; color:var(--text-gray); margin-bottom:30px;">Authenticate to manage system.</p>
        <input type="text" id="user" class="input-field" placeholder="Username">
        <input type="password" id="pass" class="input-field" placeholder="Password">
        <button class="btn-main" onclick="doLogin()">SECURE LOGIN</button>
      </div>

      <div id="admin-dash" class="hidden">
        
        <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:20px;">
           <span style="font-weight:700; font-size:14px; color:var(--text-dark)">SYSTEM OVERVIEW</span>
           <button style="background:none; border:none; color:var(--primary); font-weight:700; font-size:12px; cursor:pointer;" onclick="doLogout()">LOGOUT ➜</button>
        </div>

        <div class="admin-controls">
            <button class="btn-small active" onclick="showAdminView('grid')" id="btn-view-grid">LOCKERS</button>
            <button class="btn-small" onclick="showAdminView('history')" id="btn-view-hist">AUDIT LOGS</button>
        </div>

        <div id="view-grid">
            <div style="margin-bottom:20px; display:flex; gap:10px;">
               <button class="btn-act" style="width:100%" onclick="fetch('/unlock_all')">UNLOCK ALL</button>
               <button class="btn-act" style="width:100%" onclick="fetch('/lock_all')">LOCK ALL</button>
            </div>
            <div class="grid-admin" id="grid"></div>
        </div>

        <div id="view-history" class="hidden">
            <table>
                <thead><tr><th>Locker</th><th>Action</th><th>Actor</th></tr></thead>
                <tbody id="log-body"></tbody>
            </table>
        </div>

      </div>
    </div>
  </div>

  <div id="toast">Notification</div>

  <script>
    /* --- LOGIC PRESERVED FROM PREVIOUS VERSION --- */
    let currentCode = ""; let sessionData = {}; let isAdminAuth = false;
    let publicStatusInterval = null;

    function goToPortal(type) {
      document.getElementById('home-portal').classList.remove('active');
      if(type === 'home') {
        document.getElementById('home-portal').classList.add('active');
        document.querySelector('.tabs').classList.remove('visible');
        document.querySelectorAll('.view').forEach(v => {if(v.id !== 'home-portal') v.classList.remove('active');});
        return;
      }
      document.querySelector('.tabs').classList.add('visible');
      switchTab(type);
    }

    function checkAdminTab() {
        switchTab('admin');
        if(isAdminAuth) {
            document.getElementById('login-screen').classList.add('hidden');
            document.getElementById('admin-dash').classList.remove('hidden');
            refreshAdmin();
        } else {
            document.getElementById('login-screen').classList.remove('hidden');
            document.getElementById('admin-dash').classList.add('hidden');
        }
    }

    function switchTab(tab) {
      document.querySelectorAll('.view').forEach(v => v.classList.remove('active'));
      document.querySelectorAll('.tab').forEach(t => t.classList.remove('active'));
      document.getElementById(tab).classList.add('active');
      
      if(tab === 'customer') {
          document.getElementById('tab-cus').classList.add('active');
          refreshPublicStatus();
          if(!publicStatusInterval) publicStatusInterval = setInterval(refreshPublicStatus, 5000);
      } else {
          clearInterval(publicStatusInterval);
          publicStatusInterval = null;
      }
      if(tab === 'admin') document.getElementById('tab-adm').classList.add('active');
    }

    async function refreshPublicStatus() {
        try {
            const res = await fetch('/public_status');
            const data = await res.json();
            const grid = document.getElementById('public-grid');
            grid.innerHTML = "";
            for(let i=1; i<=15; i++) {
                const isBusy = data[i]; 
                grid.innerHTML += `<div class="dot ${isBusy ? 'busy' : 'free'}" title="Locker ${i}">${i}</div>`;
            }
        } catch(e) {}
    }

    function showAdminView(v) {
        document.getElementById('view-grid').classList.toggle('hidden', v !== 'grid');
        document.getElementById('view-history').classList.toggle('hidden', v !== 'history');
        
        document.getElementById('btn-view-grid').className = v === 'grid' ? 'btn-small active' : 'btn-small';
        document.getElementById('btn-view-hist').className = v === 'history' ? 'btn-small active' : 'btn-small';
        
        if(v === 'history') refreshLogs();
    }

    function num(n) { if(currentCode.length < 4) currentCode += n; updateDisp(); }
    function clr() { currentCode = ""; updateDisp(); }
    function updateDisp() { 
        const el = document.getElementById('disp'); 
        if(currentCode.length === 0) el.innerText = "----";
        else el.innerText = "*".repeat(currentCode.length).padEnd(4, '-');
        el.style.color = currentCode.length === 4 ? 'var(--text-dark)' : 'var(--text-gray)';
    }

    async function useCode() {
      if(currentCode.length !== 4) return showToast("Please enter 4 digits");
      const res = await fetch(`/use_code?code=${currentCode}`);
      const data = await res.json();
      if(data.success) {
        sessionData = { id: data.locker, token: data.token };
        document.getElementById('step-keypad').classList.add('hidden');
        document.getElementById('step-dash').classList.remove('hidden');
        document.getElementById('dash-id').textContent = sessionData.id;
        currentCode = ""; updateDisp();
      } else { showToast("Access Denied"); currentCode = ""; updateDisp(); }
    }

    async function lockAndLeave() {
      await fetch(`/customer_lock?id=${sessionData.id}&token=${sessionData.token}`);
      showToast("Locker Locked");
      setTimeout(() => {
        sessionData = {}; currentCode = ""; updateDisp();
        document.getElementById('step-dash').classList.add('hidden');
        document.getElementById('step-keypad').classList.remove('hidden');
        goToPortal('home');
      }, 2000);
    }

    async function doLogin() {
      const u = document.getElementById('user').value; const p = document.getElementById('pass').value;
      const res = await fetch(`/login?user=${u}&pass=${p}`);
      const data = await res.json();
      if(data.success) {
        isAdminAuth = true;
        document.getElementById('login-screen').classList.add('hidden');
        document.getElementById('admin-dash').classList.remove('hidden');
        refreshAdmin();
        setInterval(refreshAdmin, 4000);
      } else showToast("Invalid Credentials");
    }

    function doLogout() {
        isAdminAuth = false; document.getElementById('user').value = ""; document.getElementById('pass').value = "";
        document.getElementById('admin-dash').classList.add('hidden');
        document.getElementById('login-screen').classList.remove('hidden');
        showToast("Logged Out");
    }

    async function refreshAdmin() {
      if(!isAdminAuth || document.getElementById('view-grid').classList.contains('hidden')) return;
      const res = await fetch('/status');
      const data = await res.json();
      const grid = document.getElementById('grid'); grid.innerHTML = "";
      for(let i=1; i<=15; i++) {
        const l = data[i];
        const statusClass = l.state==='unlocked' ? 'unlocked' : 'locked';
        grid.innerHTML += `
        <div class="card ${statusClass}">
            <div class="card-top"><span>LOCKER ${String(i).padStart(2,'0')}</span><span>${l.state==='unlocked'?'OPEN':'LOCKED'}</span></div>
            <div class="pin-code">${l.code||'----'}</div>
            <button class="btn-act btn-gen" onclick="fetch('/gen_code?id=${i}').then(refreshAdmin)">GENERATE CODE</button>
            <div class="card-actions">
              <button class="btn-act" onclick="fetch('/admin_unlock?id=${i}').then(refreshAdmin)">UNLOCK</button>
              <button class="btn-act" onclick="fetch('/admin_lock?id=${i}').then(refreshAdmin)">LOCK</button>
            </div>
        </div>`;
      }
    }

    async function refreshLogs() {
        const res = await fetch('/logs'); const data = await res.json();
        const tbody = document.getElementById('log-body'); tbody.innerHTML = "";
        data.forEach(log => {
            const badge = log.who.includes('Admin') ? 'admin' : 'user';
            tbody.innerHTML += `<tr><td>#${log.id}</td><td>${log.act}</td><td><span class="badge ${badge}">${log.who}</span></td></tr>`;
        });
    }

    function showToast(m) { const t=document.getElementById('toast'); t.textContent=m; t.classList.add('show'); setTimeout(()=>t.classList.remove('show'), 2500); }
  </script>
</body>
</html>
)rawliteral";