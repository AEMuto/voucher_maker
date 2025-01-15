#include "MemoryMonitor.h"

// Initialize static members
size_t MemoryMonitor::lowestHeap = 0;
size_t MemoryMonitor::lowestPSRAM = 0;
unsigned long MemoryMonitor::lastCheck = 0;

void MemoryMonitor::begin() {
  lowestHeap = ESP.getFreeHeap();
  lowestPSRAM = ESP.getFreePsram();
  lastCheck = 0;

  Serial.println("Memory Monitor Initialized");
  printStats(true);
}

MemoryMonitor::MemoryStats MemoryMonitor::getStats() {
  MemoryStats stats;
  stats.freeHeap = ESP.getFreeHeap();
  stats.freePSRAM = ESP.getFreePsram();
  stats.largestHeapBlock = ESP.getMaxAllocHeap();
  stats.heapFragmentation = 100 - (stats.largestHeapBlock * 100) / stats.freeHeap;
  stats.lowestHeapSeen = lowestHeap;
  stats.lowestPSRAMSeen = lowestPSRAM;

  // Update lowest seen values
  if (stats.freeHeap < lowestHeap) lowestHeap = stats.freeHeap;
  if (stats.freePSRAM < lowestPSRAM) lowestPSRAM = stats.freePSRAM;

  return stats;
}

bool MemoryMonitor::isMemoryLow(size_t threshold) {
  return ESP.getFreeHeap() < threshold;
}

void MemoryMonitor::printStats(bool forceOutput) {
  unsigned long now = millis();
  if (forceOutput || (now - lastCheck >= CHECK_INTERVAL)) {
    lastCheck = now;
    MemoryStats stats = getStats();

    Serial.println("\n=== Memory Status ===");
    Serial.printf("Free Heap: %u bytes\n", stats.freeHeap);
    Serial.printf("Largest Free Block: %u bytes\n", stats.largestHeapBlock);
    Serial.printf("Heap Fragmentation: %u%%\n", stats.heapFragmentation);
    Serial.printf("Free PSRAM: %u bytes\n", stats.freePSRAM);
    Serial.printf("Lowest Heap Ever: %u bytes\n", stats.lowestHeapSeen);
    Serial.printf("Lowest PSRAM Ever: %u bytes\n", stats.lowestPSRAMSeen);
    Serial.println("==================\n");
  }
}