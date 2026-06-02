import { useEffect, useState } from "react";
import "./App.css";

export default function App() {
  const [state, setState] = useState({
    connected: false,
    position: [0, 0, 0],
    velocity: [0, 0, 0],
    fuel: 1.0
  });

  useEffect(() => {
    const ws = new WebSocket("ws://localhost:8080");

    ws.onopen = () => {
      setState((s) => ({ ...s, connected: true }));
    };

    ws.onmessage = (event) => {
      try {
        const data = JSON.parse(event.data);
        setState(data);
      } catch (e) {
        console.error("Bad data:", e);
      }
    };

    ws.onclose = () => {
      setState((s) => ({ ...s, connected: false }));
    };

    return () => ws.close();
  }, []);

  return (
    <div className="app">
      <header className="topbar">
        <div className={state.connected ? "dot green" : "dot red"} />
        <span>
          {state.connected ? "CONNECTED" : "DISCONNECTED"}
        </span>
      </header>

      <div className="layout">
        <div className="viewport">
          <div className="placeholder">
            <h2>3D Viewport</h2>
            <p>Three.js scene will go here</p>
          </div>
        </div>

        <div className="panel">
          <h3>Telemetry</h3>

          <div className="box">
            <p><b>Position</b></p>
            <pre>{JSON.stringify(state.position, null, 2)}</pre>
          </div>

          <div className="box">
            <p><b>Velocity</b></p>
            <pre>{JSON.stringify(state.velocity, null, 2)}</pre>
          </div>

          <div className="box">
            <p><b>Fuel</b></p>
            <p>{state.fuel}</p>
          </div>
        </div>
      </div>
    </div>
  );
}