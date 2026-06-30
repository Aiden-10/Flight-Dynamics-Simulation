import { useState } from "react";
import { useSocket } from "./hooks/useSocket";
import { StatusBar } from "./components/StatusBar";
import { TelemetryPanel } from "./components/TelemetryPanel";
import  TitleBar  from "./components/TitleBar";
import  FlightScene from "./scenes/FlightScene";
import SimConfig from "./components/SimConfig";

import "./App.css";

export default function App() {
  const { state, connected } = useSocket();
  const [settingsOpen, setSettingsOpen] = useState(false);

  return (
    <div className="app">
      <TitleBar />
      <StatusBar
        connected={connected}
        onOpenSettings={() => setSettingsOpen(true)}
      />

      <div className="layout">
        <FlightScene state={state} />
        <TelemetryPanel state={state} />
      </div>

      {settingsOpen && (
        <SimConfig onClose={() => setSettingsOpen(false)} />
      )}
    </div>
  );
}