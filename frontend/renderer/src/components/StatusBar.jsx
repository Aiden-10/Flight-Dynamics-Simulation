export function StatusBar({ connected }) {
  return (
    <div className="topbar">
      <div className={connected ? "dot green" : "dot red"} />
      {connected ? "CONNECTED" : "DISCONNECTED"}
    </div>
  );
}