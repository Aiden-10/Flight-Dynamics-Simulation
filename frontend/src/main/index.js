import { app, shell, BrowserWindow, ipcMain } from 'electron'
import { join } from 'path'
import { electronApp, optimizer, is } from '@electron-toolkit/utils'
import icon from '../../resources/icon.png?asset'
import net from 'net'

// Global reference to the main window and TCP client
let mainWindow
let tcpClient = null
let isConnected = false

// Function to connect to the C++ engine via TCP
function connectToEngine() {
  tcpClient = new net.Socket()

  tcpClient.connect(8080, '127.0.0.1', () => {
    console.log('Connected to C++ engine')
    isConnected = true
  })

 tcpClient.on('data', (data) => {
    const message = data.toString()
    // console.log('Received from C++:', message)

    if (mainWindow && !mainWindow.isDestroyed()) {
      mainWindow.webContents.send('telemetry-update', message)
    }
  })

  tcpClient.on('error', () => {
    console.log('Engine disconnected or not found. Retrying in 3 seconds...')
    isConnected = false
    tcpClient.destroy()
    setTimeout(connectToEngine, 3000)
  })

  tcpClient.on('close', () => {
    isConnected = false
  })
}


function createWindow() {
  // Create the browser window.
  mainWindow = new BrowserWindow({
    width: 900,
    height: 670,
    frame: false,
    titleBarStyle: "hidden",
    autoHideMenuBar: true,
    ...(process.platform === 'linux' ? { icon } : {}),
    webPreferences: {
      preload: join(__dirname, '../preload/index.js'),
      contextIsolation: true,
      nodeIntegration: false,
      sandbox: false
    }
  })

  // Connect to the C++ engine
  connectToEngine()

  // IPC handlers for window controls
  ipcMain.on("window:minimize", () => {
    mainWindow.minimize();
  });
  ipcMain.on("window:maximize", () => {
    if (mainWindow.isMaximized()) {
      mainWindow.unmaximize();
    } else {
      mainWindow.maximize();
    }
  });
  ipcMain.on("window:close", () => {
    mainWindow.close();
  });

  // Show the window
  mainWindow.on('ready-to-show', () => {
    mainWindow.show()
  })
  mainWindow.webContents.setWindowOpenHandler((details) => {
    shell.openExternal(details.url)
    return { action: 'deny' }
  })
  if (is.dev && process.env['ELECTRON_RENDERER_URL']) {
    mainWindow.loadURL(process.env['ELECTRON_RENDERER_URL'])
  } else {
    mainWindow.loadFile(join(__dirname, '../renderer/index.html'))
  }
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(() => {
  // Set app user model id for windows
  electronApp.setAppUserModelId('com.electron')

  // Default open or close DevTools by F12 in development
  // and ignore CommandOrControl + R in production.
  // see https://github.com/alex8088/electron-toolkit/tree/master/packages/utils
  app.on('browser-window-created', (_, window) => {
    optimizer.watchWindowShortcuts(window)
  })

  // IPC test
  ipcMain.on('ping', () => console.log('pong'))

  ipcMain.on('send-command-to-engine', (_event, command) => {
    console.log('Command received in main:', command)

    if (!tcpClient || !isConnected || tcpClient.destroyed) {
      console.log('TCP client not connected')
      return
    }

    const message = JSON.stringify(command) + '\n'
    tcpClient.write(message)

    console.log('Sent to C++:', message)
  })

  createWindow()

  app.on('activate', function () {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})

// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})

