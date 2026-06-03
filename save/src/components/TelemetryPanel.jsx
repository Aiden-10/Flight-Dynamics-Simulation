import { useState } from "react";
import styles from "./TelemetryPanel.module.css";

// Helper component for the foldable sections
function Section({ title, defaultOpen = false, children }) {
  const [isOpen, setIsOpen] = useState(defaultOpen);

  return (
    <div className={styles.section}>
      <button 
        className={`${styles.sectionHeader} ${isOpen ? styles.open : ""}`} 
        onClick={() => setIsOpen(!isOpen)}
      >
        <span className={styles.sectionTitle}>{title}</span>
        <span className={styles.caret}>{isOpen ? "▼" : "▶"}</span>
      </button>
      {isOpen && <div className={styles.sectionContent}>{children}</div>}
    </div>
  );
}

export function TelemetryPanel({ state }) {
  return (
    <div className={styles.panel}>
      
      {/* ENVIRONMENT SECTION */}
      <Section title="Atmosphere" defaultOpen={true}>
        <div className={styles.formLayout}>
          <div className={styles.inputGroup}>
            <label>Crosswind (knots)</label>
            <input type="range" min="0" max="150" defaultValue="0" />
          </div>
          <div className={styles.inputGroup}>
            <label>Gravity Mode</label>
            <select className={styles.darkSelect} defaultValue="earth">
              <option value="earth">Earth (9.81 m/s²)</option>
              <option value="mars">Mars (3.71 m/s²)</option>
              <option value="moon">Moon (1.62 m/s²)</option>
            </select>
          </div>
        </div>
      </Section>

      {/* VEHICLE SECTION */}
      <Section title="Propulsion" defaultOpen={true}>
        <div className={styles.formLayout}>
          <div className={styles.inputGroup}>
            <label>Main Throttle (%)</label>
            <input type="range" min="0" max="100" defaultValue="0" />
          </div>
          <div className={styles.inputGroup}>
            <label>Initial Fuel Mass (kg)</label>
            <input type="number" defaultValue="5000" className={styles.darkInput} />
          </div>
          <button className={styles.actionBtn}>Trigger Engine Burn</button>
        </div>
      </Section>

      {/* DISPLAY SECTION */}
      <Section title="Visual Debugging" defaultOpen={false}>
        <div className={styles.formLayout}>
          <div className={styles.toggleGroup}>
            <input type="checkbox" id="show-lift" defaultChecked />
            <label htmlFor="show-lift">Show Lift Vector</label>
          </div>
          <div className={styles.toggleGroup}>
            <input type="checkbox" id="show-drag" defaultChecked />
            <label htmlFor="show-drag">Show Drag Vector</label>
          </div>
          <div className={styles.toggleGroup}>
            <input type="checkbox" id="show-trail" />
            <label htmlFor="show-trail">Draw Flight Trail</label>
          </div>
        </div>
      </Section>

      {/* ADVANCED / DEBUG SECTION (Hidden by default) */}
      <Section title="Raw Telemetry (Debug)" defaultOpen={false}>
         {!state ? (
            <div className={styles.placeholder}>Awaiting Data...</div>
          ) : (
            <pre className={styles.jsonOutput}>{JSON.stringify(state, null, 2)}</pre>
          )}
      </Section>

    </div>
  );
}