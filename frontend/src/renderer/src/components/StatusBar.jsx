import { useState } from "react";
import styles from "./StatusBar.module.css";

export function StatusBar({ connected, onOpenSettings}) {
  return (
    <>
      <div className={styles.topbar}>
        <div className={styles.leftCluster}>
          <button className={styles.settingsBtn} onClick={onOpenSettings}>
            ⚙ SIM CONFIG
          </button>

          <div className={styles.statusIndicator}>
            <div className={`${styles.dot} ${connected ? styles.green : styles.red}`} />
            <span>{connected ? "LINK ACTIVE" : "LINK LOST"}</span>
          </div>
        </div>

        <div className={styles.centerCluster}>
          <button className={styles.controlBtn}>RESET</button>
          <button className={styles.controlBtn}>RUN</button>
          <button className={styles.controlBtn}>HOLD</button>
        </div>

        <div className={styles.rightCluster}>
          <span className={styles.readoutLabel}>SIM RATE</span>
          <select className={styles.darkSelect} defaultValue="1.0">
            <option value="0.5">0.5x</option>
            <option value="1.0">1.0x</option>
            <option value="2.0">2.0x</option>
            <option value="10.0">10.0x</option>
          </select>
        </div>
      </div>
    </>
  );
}