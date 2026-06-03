import { useEffect, useRef } from "react";
import * as THREE from "https://cdn.jsdelivr.net/npm/three@0.160.0/build/three.module.js";

export default function FlightScene({ state }) {
  const mountRef = useRef(null);
  const stateRef = useRef(state);

  // Keep animation loop updated with latest WebSocket state
  useEffect(() => {
    stateRef.current = state;
  }, [state]);

  // Engine Lifecycle
  useEffect(() => {
    const currentMount = mountRef.current;
    if (!currentMount) return;

    // --------------------
    // Initialization
    // --------------------
    const scene = new THREE.Scene();
    scene.background = new THREE.Color(0x0b0f14);

    const camera = new THREE.PerspectiveCamera(
      75,
      currentMount.clientWidth / currentMount.clientHeight,
      0.1,
      1000
    );
    camera.position.set(0, 5, 10);

    const renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(currentMount.clientWidth, currentMount.clientHeight);
    currentMount.appendChild(renderer.domElement);

    // --------------------
    // Lighting & Objects
    // --------------------
    scene.add(new THREE.AmbientLight(0xffffff, 0.4));

    const light = new THREE.DirectionalLight(0xffffff, 1);
    light.position.set(5, 10, 5);
    scene.add(light);

    const plane = new THREE.Mesh(
      new THREE.BoxGeometry(1, 0.3, 2),
      new THREE.MeshStandardMaterial({ color: 0x00ffcc })
    );
    scene.add(plane);
    scene.add(new THREE.GridHelper(200, 50));

    // --------------------
    // Resize Observer
    // --------------------
    const handleResize = () => {
      if (!currentMount) return;
      const width = currentMount.clientWidth;
      const height = currentMount.clientHeight;

      renderer.setSize(width, height);
      camera.aspect = width / height;
      camera.updateProjectionMatrix();
    };

    const resizeObserver = new ResizeObserver(handleResize);
    resizeObserver.observe(currentMount);

    // --------------------
    // Animation Loop
    // --------------------
    let animationId;
    const animate = () => {
      animationId = requestAnimationFrame(animate);

      const currentState = stateRef.current;

      // Apply the latest telemetry to the plane
      if (currentState) {
        const p = currentState.position;
        const q = currentState.quaternion;

        if (p) plane.position.set(p[0], p[1], p[2]);
        if (q) plane.quaternion.set(q[0], q[1], q[2], q[3]);
      }

      // Smooth camera follow logic
      const p = plane.position;
      camera.position.lerp(new THREE.Vector3(p.x, p.y + 5, p.z + 12), 0.08);
      camera.lookAt(p);

      renderer.render(scene, camera);
    };

    animate();

    // --------------------
    // Cleanup Phase
    // --------------------
    return () => {
      cancelAnimationFrame(animationId);
      resizeObserver.disconnect();
      renderer.dispose();
      renderer.forceContextLoss();

      if (currentMount && renderer.domElement) {
        currentMount.removeChild(renderer.domElement);
      }
    };
  }, []); // Empty dependency array ensures this setup only runs once

  return <div ref={mountRef} className="viewport" />;
}