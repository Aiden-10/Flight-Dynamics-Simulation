import { useEffect, useState } from "react";

export function useSocket() {
  const [state, setState] = useState(null);
  const [connected, setConnected] = useState(false);

  useEffect(() => {
    if (!window.engineAPI) {
      console.error("window.engineAPI is not available");
      setConnected(false);
      return;
    }

    const unsubscribe = window.engineAPI.onTelemetryUpdate((message) => {
      console.log("Raw telemetry:", message);

      try {
        const parsed = JSON.parse(message.trim());

        console.log("Parsed telemetry:", parsed);

        setState(parsed);
        setConnected(true);
      } catch (err) {
        console.error("Failed to parse telemetry:", message, err);
      }
    });

    return () => {
      if (unsubscribe) unsubscribe();
    };
  }, []);

  return { state, connected };
}