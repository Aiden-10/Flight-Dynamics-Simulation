import { ipcRenderer, contextBridge } from 'electron'
import { electronAPI } from '@electron-toolkit/preload'

// Expose a safe API to the renderer process
const api = {}

// Expose a safe API to the renderer process for engine communication
const engineAPI = {
  sendCommand: (command) => {
    ipcRenderer.send('send-command-to-engine', command)
  },

  onTelemetryUpdate: (callback) => {
    const listener = (_event, message) => {
      callback(message);
    };

    ipcRenderer.on("telemetry-update", listener);

    return () => {
      ipcRenderer.removeListener("telemetry-update", listener);
    };
  }
}

// Expose APIs to the renderer process
// if context isolation is enabled, use contextBridge to expose the APIs
if (process.contextIsolated) {
  try {
    console.log("PRELOAD LOADED")
    contextBridge.exposeInMainWorld('electron', electronAPI)
    contextBridge.exposeInMainWorld('api', api)
    contextBridge.exposeInMainWorld('engineAPI', engineAPI)

    contextBridge.exposeInMainWorld('electronWindow', {
      minimize: () => ipcRenderer.send('window:minimize'),
      maximize: () => ipcRenderer.send('window:maximize'),
      close: () => ipcRenderer.send('window:close')
    })
  } catch (error) {
    console.error(error)
  }
  // If context isolation is disabled, directly assign the APIs to the window object
} else {
  window.electron = electronAPI
  window.api = api
  window.engineAPI = engineAPI

  window.electronWindow = {
    minimize: () => ipcRenderer.send('window:minimize'),
    maximize: () => ipcRenderer.send('window:maximize'),
    close: () => ipcRenderer.send('window:close')
  }
}