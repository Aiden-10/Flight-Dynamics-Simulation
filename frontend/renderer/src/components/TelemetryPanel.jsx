export function TelemetryPanel({ state }) {
  if (!state) return <div className="panel">Waiting...</div>;

  return (
    <div className="panel">
      <h3>Telemetry</h3>

      <pre>{JSON.stringify(state, null, 2)}</pre>
    </div>
  );
}