import styles from "./StatusBar.module.css";

export function StatusBar({ connected }) {
  return (
    <div className={styles.topbar}>
      {/* LEFT: Connection Status */}
      <div className={styles.statusIndicator}>
        <div className={`${styles.dot} ${connected ? styles.green : styles.red}`} />
        <span className={styles.statusText}>
          {connected ? "CONNECTED" : "NOT CONNECTED"}
        </span>
      </div>

      {/* CENTER: Playback Controls */}
      <div className={styles.simControls}>
        <button className={styles.iconBtn} title="Reset Simulation">⏮</button>
        <button className={styles.iconBtn} title="Play">▶</button>
        <button className={styles.iconBtn} title="Pause">⏸</button>
      </div>

      {/* RIGHT: Time Scale Dropdown */}
      <div className={styles.timeScale}>
        <label>Timescale</label>
        <select className={styles.darkSelect} defaultValue="1.0">
          <option value="0.5">0.5x (Slow)</option>
          <option value="1.0">1.0x (Realtime)</option>
          <option value="2.0">2.0x</option>
          <option value="10.0">10.0x</option>
        </select>
      </div>
    </div>
  );
}