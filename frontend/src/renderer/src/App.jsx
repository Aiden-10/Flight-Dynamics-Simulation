import { useSocket } from "./hooks/useSocket";
import { StatusBar } from "./components/StatusBar";
import { TelemetryPanel } from "./components/TelemetryPanel";
import FlightScene from "./scenes/FlightScene";
import "./App.css";

export default function App() {
  const { state, connected } = useSocket();

  return (
    <div className="app">
      <StatusBar connected={connected} />

      <div className="layout">
        <FlightScene state={state} />
        <TelemetryPanel state={state} />
      </div>
    </div>
  );
}