import { useState } from "react";
import styles from "./TelemetryPanel.module.css";

function Instrument({ title, code, defaultOpen = true, children }) {
  const [isOpen, setIsOpen] = useState(defaultOpen);

  return (
    <div className={styles.instrument}>
      <button
        className={`${styles.instrumentHeader} ${isOpen ? styles.open : ""}`}
        onClick={() => setIsOpen(!isOpen)}
      >
        <span className={styles.instrumentCode}>{code}</span>
        <span className={styles.instrumentTitle}>{title}</span>
        <span className={styles.caret}>{isOpen ? "ONLINE" : "STBY"}</span>
      </button>

      {isOpen && <div className={styles.instrumentBody}>{children}</div>}
    </div>
  );
}

function Readout({ label, value, unit }) {
  return (
    <div className={styles.readout}>
      <span className={styles.readoutLabel}>{label}</span>
      <span className={styles.readoutValue}>
        {value}
        {unit && <span className={styles.unit}> {unit}</span>}
      </span>
    </div>
  );
}

export function TelemetryPanel({ state }) {
  return (
    <aside className={styles.panel}>
      <div className={styles.panelTitle}>
        <span>FLIGHT DATA</span>
        <span className={styles.panelStatus}>LIVE</span>
      </div>

      <Instrument title="Flight State" code="FLT">
        <div className={styles.readoutGrid}>
          <Readout label="Altitude" value={state?.altitude?.toFixed?.(1) ?? "----"} unit="m" />
          <Readout label="Velocity" value={state?.velocity?.toFixed?.(1) ?? "----"} unit="m/s" />
          <Readout label="Pitch" value={state?.pitch?.toFixed?.(1) ?? "----"} unit="deg" />
          <Readout label="AoA" value={state?.aoa?.toFixed?.(1) ?? "----"} unit="deg" />
        </div>
      </Instrument>

      <Instrument title="Atmosphere" code="ATM">
        <div className={styles.controlBlock}>
          <label>Crosswind</label>
          <input type="range" min="0" max="150" defaultValue="0" />
        </div>

        <div className={styles.controlBlock}>
          <label>Gravity Body</label>
          <select className={styles.darkSelect} defaultValue="earth">
            <option value="earth">Earth / 9.81 m/s²</option>
            <option value="mars">Mars / 3.71 m/s²</option>
            <option value="moon">Moon / 1.62 m/s²</option>
          </select>
        </div>
      </Instrument>

      <Instrument title="Propulsion" code="ENG">
        <div className={styles.controlBlock}>
          <label>Main Throttle</label>
          <input type="range" min="0" max="100" defaultValue="0" />
        </div>

        <div className={styles.controlBlock}>
          <label>Initial Fuel Mass</label>
          <input type="number" defaultValue="5000" className={styles.darkInput} />
        </div>

        <button className={styles.armButton}>ARM ENGINE BURN</button>
      </Instrument>

      <Instrument title="Vector Display" code="VEC" defaultOpen={false}>
        <div className={styles.toggleGroup}>
          <input type="checkbox" id="show-lift" defaultChecked />
          <label htmlFor="show-lift">Lift Vector</label>
        </div>

        <div className={styles.toggleGroup}>
          <input type="checkbox" id="show-drag" defaultChecked />
          <label htmlFor="show-drag">Drag Vector</label>
        </div>

        <div className={styles.toggleGroup}>
          <input type="checkbox" id="show-trail" />
          <label htmlFor="show-trail">Flight Trail</label>
        </div>
      </Instrument>

      <Instrument title="Raw Packet" code="DBG" defaultOpen={false}>
        {!state ? (
          <div className={styles.placeholder}>NO DATA FRAME RECEIVED</div>
        ) : (
          <pre className={styles.jsonOutput}>{JSON.stringify(state, null, 2)}</pre>
        )}
      </Instrument>
    </aside>
  );
}