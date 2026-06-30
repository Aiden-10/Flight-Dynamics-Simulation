import { useState } from "react";
import styles from "./SimConfig.module.css";

const cardTemplates = {
  vehicle: {
    title: "VEHICLE",
    fields: [
      { name: "dryMass", label: "Dry Mass / kg", type: "number", defaultValue: 1200 },
      { name: "fuelMass", label: "Fuel Mass / kg", type: "number", defaultValue: 5000 },
      { name: "referenceArea", label: "Reference Area / m²", type: "number", defaultValue: 0.045 },
      { name: "rocketCount", label: "Rocket Count", type: "number", defaultValue: 1, min: 1 },
    ],
  },

  propulsion: {
    title: "PROPULSION",
    fields: [
      { name: "maxThrust", label: "Max Thrust / N", type: "number", defaultValue: 250000 },
      { name: "burnTime", label: "Burn Time / s", type: "number", defaultValue: 60 },
      { name: "specificImpulse", label: "Specific Impulse / s", type: "number", defaultValue: 280 },
      { name: "throttleLimit", label: "Throttle Limit / %", type: "number", defaultValue: 100 },
    ],
  },

  atmosphere: {
    title: "ATMOSPHERE",
    fields: [
      {
        name: "atmosphereModel",
        label: "Atmosphere Model",
        type: "select",
        defaultValue: "standard",
        options: [
          { value: "standard", label: "US Standard Atmosphere" },
          { value: "constant", label: "Constant Density" },
          { value: "vacuum", label: "Vacuum" },
        ],
      },
      {
        name: "windModel",
        label: "Wind Model",
        type: "select",
        defaultValue: "none",
        options: [
          { value: "none", label: "None" },
          { value: "constant", label: "Constant Wind" },
          { value: "gust", label: "Gust Model" },
          { value: "shear", label: "Altitude Shear" },
        ],
      },
      { name: "crosswind", label: "Crosswind / knots", type: "number", defaultValue: 0 },
    ],
  },

  environment: {
    title: "ENVIRONMENT",
    fields: [
      {
        name: "gravityBody",
        label: "Gravity Body",
        type: "select",
        defaultValue: "earth",
        options: [
          { value: "earth", label: "Earth" },
          { value: "moon", label: "Moon" },
          { value: "mars", label: "Mars" },
        ],
      },
    ],
  },

  guidance: {
    title: "GUIDANCE",
    fields: [
      {
        name: "controlMode",
        label: "Control Mode",
        type: "select",
        defaultValue: "manual",
        options: [
          { value: "manual", label: "Manual" },
          { value: "stability", label: "Stability Assist" },
          { value: "trajectory", label: "Trajectory Follow" },
        ],
      },
      { name: "targetAltitude", label: "Target Altitude / m", type: "number", defaultValue: 10000 },
      { name: "launchAngle", label: "Launch Angle / deg", type: "number", defaultValue: 90 },
      {
        name: "integrator",
        label: "Integrator",
        type: "select",
        defaultValue: "rk4",
        options: [
          { value: "euler", label: "Euler" },
          { value: "rk4", label: "RK4" },
        ],
      },
    ],
  },
};

const defaultCards = [
  { id: crypto.randomUUID(), type: "vehicle", ...cardTemplates.vehicle },
  { id: crypto.randomUUID(), type: "propulsion", ...cardTemplates.propulsion },
  { id: crypto.randomUUID(), type: "atmosphere", ...cardTemplates.atmosphere },
  { id: crypto.randomUUID(), type: "guidance", ...cardTemplates.guidance },
];

export default function SimConfig({ onClose }) {
  const [cards, setCards] = useState(defaultCards);
  const [selectedType, setSelectedType] = useState("vehicle");

  function addCard() {
    const template = cardTemplates[selectedType];

    setCards((prev) => [
      ...prev,
      {
        id: crypto.randomUUID(),
        type: selectedType,
        ...template,
      },
    ]);
  }

  function removeCard(id) {
    setCards((prev) => prev.filter((card) => card.id !== id));
  }

  return (
    <div className={styles.modalOverlay}>
      <div className={styles.configModal}>
        <div className={styles.modalHeader}>
          <div>
            <div className={styles.modalTitle}>SIMULATION CONFIG</div>
            <div className={styles.modalSubtitle}>
              Vehicle, engine, and environment parameters
            </div>
          </div>

          <button className={styles.closeBtn} onClick={onClose}>
            CLOSE
          </button>
        </div>

        <div className={styles.modalGrid}>
          {cards.map((card) => (
            <ConfigCard
              key={card.id}
              card={card}
              onRemove={() => removeCard(card.id)}
            />
          ))}

          <div className={styles.addCard}>
            <div className={styles.plus}>+</div>
            <div className={styles.addTitle}>ADD MODULE</div>

            <select
              className={styles.addSelect}
              value={selectedType}
              onChange={(e) => setSelectedType(e.target.value)}
            >
              <option value="vehicle">Vehicle</option>
              <option value="propulsion">Propulsion</option>
              <option value="atmosphere">Atmosphere</option>
              <option value="environment">Environment</option>
              <option value="guidance">Guidance</option>
            </select>

            <button className={styles.addBtn} onClick={addCard}>
              ADD
            </button>
          </div>
        </div>

        <div className={styles.modalFooter}>
          <button className={styles.secondaryBtn}>LOAD PRESET</button>
          <button className={styles.secondaryBtn}>SAVE PRESET</button>
          <button className={styles.applyBtn} onClick={onClose}>
            APPLY CONFIG
          </button>
        </div>
      </div>
    </div>
  );
}

function ConfigCard({ card, onRemove }) {
  return (
    <section className={styles.configSection}>
      <div className={styles.cardHeader}>
        <h3>{card.title}</h3>
        <button className={styles.removeBtn} onClick={onRemove}>
          ×
        </button>
      </div>

      {card.fields.map((field) => (
        <Field key={field.name} field={field} />
      ))}
    </section>
  );
}

function Field({ field }) {
  return (
    <div className={styles.fieldGroup}>
      <label>{field.label}</label>

      {field.type === "select" ? (
        <select defaultValue={field.defaultValue}>
          {field.options.map((option) => (
            <option key={option.value} value={option.value}>
              {option.label}
            </option>
          ))}
        </select>
      ) : (
        <input
          type={field.type}
          defaultValue={field.defaultValue}
          min={field.min}
        />
      )}
    </div>
  );
}