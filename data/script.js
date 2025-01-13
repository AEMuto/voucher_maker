// script.js
document.addEventListener("DOMContentLoaded", () => {
    const ssidSelect = document.getElementById("ssid");
    const manualCheckbox = document.getElementById("manualEntry");
    const manualSsidInput = document.getElementById("manualSsid");
    const statusDiv = document.getElementById("status");
    const scanStatus = document.querySelector(".scan-status");
    const passwordInput = document.getElementById("password");
    const togglePasswordBtn = document.getElementById("togglePassword");
    
    let isScanning = false;
    
    async function scanNetworks() {
        if (isScanning) return;
        
        isScanning = true;
        ssidSelect.disabled = true;
        scanStatus.textContent = "Scanning for networks...";
        scanStatus.className = "scan-status scanning";
        
        try {
            const response = await fetch("/scan-wifi");
            const networks = await response.json();
            
            // Clear existing options except the placeholder
            while (ssidSelect.options.length > 1) {
                ssidSelect.remove(1);
            }
            
            // Add found networks
            networks
                .sort((a, b) => b.rssi - a.rssi) // Sort by signal strength
                .forEach(network => {
                    const option = new Option(
                        `${network.ssid} (${network.rssi}dBm)`, 
                        network.ssid
                    );
                    option.dataset.strength = getSignalStrength(network.rssi);
                    ssidSelect.add(option);
                });
            
            scanStatus.textContent = `Found ${networks.length} networks`;
            networks.length === 0 && (scanStatus.textContent = "No networks found. Click to try again.");
            
        } catch (error) {
            scanStatus.textContent = "Scan failed. Click to try again.";
            console.error("Scan error:", error);
        } finally {
            isScanning = false;
            ssidSelect.disabled = false;
        }
    }
    
    // Scan when select is clicked/focused
    ssidSelect.addEventListener("mousedown", (e) => {
        if (!isScanning && ssidSelect.options.length <= 1) {
            e.preventDefault(); // Prevent showing empty dropdown
            scanNetworks();
        }
    });
    
    ssidSelect.addEventListener("focus", () => {
        if (!isScanning && ssidSelect.options.length <= 1) {
            scanNetworks();
        }
    });
    
    // Toggle password visibility
    togglePasswordBtn.addEventListener("click", () => {
        const type = passwordInput.type === "password" ? "text" : "password";
        passwordInput.type = type;
        togglePasswordBtn.textContent = type === "password" ? "Show" : "Hide";
    });
    
    manualCheckbox.addEventListener("change", () => {
        ssidSelect.disabled = manualCheckbox.checked;
        ssidSelect.required = !manualCheckbox.checked;
        scanStatus.style.display = manualCheckbox.checked ? "none" : "block";
        manualSsidInput.placeholder = manualCheckbox.checked ? "e.g. MyNetwork" : "Disabled";
        manualSsidInput.disabled = !manualCheckbox.checked;
        manualSsidInput.required = manualCheckbox.checked;
    });
    
    document.getElementById("wifiForm").addEventListener("submit", async (e) => {
        e.preventDefault();
        
        const ssid = manualCheckbox.checked ? manualSsidInput.value : ssidSelect.value;
        const password = passwordInput.value;
        
        if (!ssid) {
            statusDiv.textContent = "Please select or enter a network name.";
            statusDiv.className = "error";
            return;
        }
        
        try {
            const response = await fetch("/save-credentials", {
                method: "POST",
                headers: {
                    "Content-Type": "application/json",
                },
                body: JSON.stringify({ ssid, password })
            });
            
            if (response.ok) {
                statusDiv.textContent = "WiFi credentials saved successfully!";
                statusDiv.className = "success";
            } else {
                throw new Error("Failed to save credentials");
            }
        } catch (error) {
            statusDiv.textContent = "Error saving credentials. Please try again.";
            statusDiv.className = "error";
        }
    });
    
    function getSignalStrength(rssi) {
        if (rssi >= -50) return "strong";
        if (rssi >= -70) return "medium";
        return "weak";
    }
});