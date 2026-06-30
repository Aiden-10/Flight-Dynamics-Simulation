import styles from "./TitleBar.module.css";

export default function TitleBar() {
  const minimize = () => window.electronWindow?.minimize?.();
  const maximize = () => window.electronWindow?.maximize?.();
  const close = () => window.electronWindow?.close?.();

  return (
    <header className={styles.titleBar}>
      <div className={styles.left}>

        <span className={styles.title}>
          3D ROCKET SIMULATION
        </span>

        <span className={styles.version}>
          AIDEN SLABIAK
        </span>
      </div>

      <div className={styles.center}>
        
      </div>

      <div className={styles.windowControls}>
        <button
          className={styles.windowBtn}
          onClick={minimize}
          title="Minimize"
        >
          —
        </button>

        <button
          className={styles.windowBtn}
          onClick={maximize}
          title="Maximize"
        >
          □
        </button>

        <button
          className={`${styles.windowBtn} ${styles.close}`}
          onClick={close}
          title="Close"
        >
          ✕
        </button>
      </div>
    </header>
  );
}