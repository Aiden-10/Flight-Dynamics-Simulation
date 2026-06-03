import { useEffect, useState } from "react";

export function useSocket() {
  const [state, setState] = useState({
    position: [0, 2, 0],
    quaternion: [0, 0, 0, 1],
    velocity: [1, 0, 0],
  });
  
  // Added a connected state to fulfill the App.js destructuring
  const [connected, setConnected] = useState(false);

  useEffect(() => {
    setConnected(true); // Mock connection established
    let t = 0;
    let raf;

    const update = () => {
      t += 0.01;

      const x = Math.sin(t) * 10;
      const z = Math.cos(t) * 10;

      setState({
        position: [x, 2, z],
        quaternion: [0, Math.sin(t * 0.5), 0, 1],
        velocity: [1, 0, 1],
      });

      raf = requestAnimationFrame(update);
    };

    update();

    return () => cancelAnimationFrame(raf);
  }, []);

  // IMPORTANT FIX: Return as an object so App.js can destructure it
  return { state, connected };
}