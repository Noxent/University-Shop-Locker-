#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <DNSServer.h>  // <--- NEW: Required for Captive Portal
#include <vector>
#include "index.h"      // Your UI file (Keep the one with the Logo!)

// =====================
// Settings
// =====================
const char* AP_SSID = "U-Shop Smart Locker - 01";
const char* AP_PASS = "12345678"; // (Optional: Remove password for true 'Hotel Style' open portal)

const char* ADMIN_USER = "admin";
const char* ADMIN_PASS = "admin123"; 

const uint8_t LOCKER_PINS[15] = {
  23, 22, 21, 19, 18, 5, 
  17, 16, 4, 27, 26, 25, 
  33, 32, 14
};

const bool RELAY_ACTIVE_LOW = true; 

// =====================
// Data Structures & Globals
// =====================
const byte DNS_PORT = 53; // <--- NEW: standard DNS port
DNSServer dnsServer;      // <--- NEW: The DNS object

struct AuditLog {
  int lockerId;
  String action;
  String actor;
};

bool lockerUnlocked[15] = {0};
String lockerOTPs[15];      
String lockerSessions[15];  
std::vector<AuditLog> logs; 
const int MAX_LOGS = 50;

WebServer server(80);
Preferences preferences;

// =====================
// Helpers
// =====================
void addLog(int id, String act, String who) {
  if (logs.size() >= MAX_LOGS) logs.pop_back();
  logs.insert(logs.begin(), {id, act, who});
}

void setLocker(uint8_t id, bool unlock, String method) {
  if (id < 1 || id > 15) return;
  uint8_t idx = id - 1;
  lockerUnlocked[idx] = unlock;
  bool pinLevel = unlock; 
  if (RELAY_ACTIVE_LOW) pinLevel = !pinLevel;
  digitalWrite(LOCKER_PINS[idx], pinLevel ? HIGH : LOW);
  addLog(id, unlock ? "Unlocked" : "Locked", method);
}

void loadSavedCodes() {
  preferences.begin("lockers", false);
  for (int i = 0; i < 15; i++) {
    String key = "otp" + String(i + 1);
    String savedCode = preferences.getString(key.c_str(), "");
    if (savedCode != "") lockerOTPs[i] = savedCode;
  }
  preferences.end();
}

// =====================
// Web Routes
// =====================
void handleRoot() { server.send(200, "text/html", INDEX_HTML); }

void handleLogin() {
  if (server.arg("user") == ADMIN_USER && server.arg("pass") == ADMIN_PASS) 
    server.send(200, "application/json", "{\"success\":true}");
  else 
    server.send(200, "application/json", "{\"success\":false}");
}

void handlePublicStatus() {
  String json = "{";
  for (int i = 0; i < 15; i++) {
    // Occupied if PIN exists OR Session exists
    bool isOccupied = (lockerOTPs[i].length() > 0) || (lockerSessions[i].length() > 0);
    json += "\"" + String(i+1) + "\":" + (isOccupied ? "true" : "false");
    if (i < 14) json += ",";
  }
  json += "}";
  server.send(200, "application/json", json);
}

void handleGetLogs() {
  String json = "[";
  for (size_t i = 0; i < logs.size(); i++) {
    json += "{\"id\":" + String(logs[i].lockerId) + ",";
    json += "\"act\":\"" + logs[i].action + "\",";
    json += "\"who\":\"" + logs[i].actor + "\"}";
    if (i < logs.size() - 1) json += ",";
  }
  json += "]";
  server.send(200, "application/json", json);
}

void handleStatus() {
  String json = "{";
  for (uint8_t i = 1; i <= 15; i++) {
    uint8_t idx = i - 1;
    json += "\"" + String(i) + "\":{";
    json += "\"state\":\"" + String(lockerUnlocked[idx] ? "unlocked" : "locked") + "\",";
    json += "\"code\":\"" + lockerOTPs[idx] + "\""; 
    json += "}";
    if (i != 15) json += ",";
  }
  json += "}";
  server.send(200, "application/json", json);
}

void handleGenCode() {
  if (!server.hasArg("id")) return server.send(400, "text/plain", "Missing id");
  int id = server.arg("id").toInt();
  String code = String(random(1000, 9999));
  
  lockerOTPs[id - 1] = code; 
  lockerSessions[id - 1] = ""; 
  preferences.begin("lockers", false);
  preferences.putString(("otp" + String(id)).c_str(), code);
  preferences.end();

  addLog(id, "New PIN Generated", "Admin");
  server.send(200, "application/json", "{\"code\":\"" + code + "\"}");
}

void handleUseCode() {
  String inputCode = server.arg("code");
  for (int i = 0; i < 15; i++) {
    if (lockerOTPs[i].length() == 4 && lockerOTPs[i] == inputCode) {
      setLocker(i + 1, true, "Customer");
      lockerOTPs[i] = ""; 
      preferences.begin("lockers", false);
      preferences.remove(("otp" + String(i + 1)).c_str());
      preferences.end();
      String token = String(random(10000, 99999));
      lockerSessions[i] = token;
      String json = "{\"success\":true, \"locker\":" + String(i+1) + ", \"token\":\"" + token + "\"}";
      server.send(200, "application/json", json);
      return;
    }
  }
  server.send(200, "application/json", "{\"success\":false}");
}

void handleCustomerLock() {
  int id = server.arg("id").toInt();
  String token = server.arg("token");
  int idx = id - 1;

  if (id >= 1 && id <= 15 && lockerSessions[idx] == token) {
    setLocker(id, false, "Customer");
    lockerSessions[idx] = ""; 
    server.send(200, "application/json", "{\"success\":true}");
  } else {
    server.send(403, "application/json", "{\"success\":false}");
  }
}

void handleAdminUnlock() {
  if (!server.hasArg("id")) return;
  setLocker(server.arg("id").toInt(), true, "Admin");
  server.send(200, "text/plain", "OK");
}

void handleAdminLock() {
  if (!server.hasArg("id")) return;
  setLocker(server.arg("id").toInt(), false, "Admin");
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0)); 
  
  for (uint8_t i = 0; i < 15; i++) {
    pinMode(LOCKER_PINS[i], OUTPUT);
    digitalWrite(LOCKER_PINS[i], RELAY_ACTIVE_LOW ? HIGH : LOW);
  }
  loadSavedCodes(); 
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);

  // --- NEW: START CAPTIVE PORTAL DNS ---
  // Redirect all traffic ("*") to the ESP32's own IP
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP()); 

  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/status", handleStatus);
  server.on("/public_status", handlePublicStatus);
  server.on("/logs", handleGetLogs);
  server.on("/gen_code", handleGenCode);
  server.on("/admin_unlock", handleAdminUnlock);
  server.on("/admin_lock", handleAdminLock);
  server.on("/use_code", handleUseCode);       
  server.on("/customer_lock", handleCustomerLock); 
  server.on("/unlock_all", [](){ for(int i=1;i<=15;i++) setLocker(i, true, "Admin Master"); server.send(200); });
  server.on("/lock_all", [](){ for(int i=1;i<=15;i++) setLocker(i, false, "Admin Master"); server.send(200); });

  // --- NEW: CATCH ALL "INTERNET CHECKS" ---
  // If Android/iOS checks for internet (e.g., generate_204), send them the Locker UI instead.
  server.onNotFound([]() {
    server.send(200, "text/html", INDEX_HTML);
  });

  server.begin();
}

void loop() {
  // --- NEW: PROCESS DNS REQUESTS ---
  dnsServer.processNextRequest();
  server.handleClient();
}