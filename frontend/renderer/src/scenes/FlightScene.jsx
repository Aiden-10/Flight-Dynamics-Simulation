import { useEffect, useRef } from "react";
import * as THREE from "https://cdn.jsdelivr.net/npm/three@0.160.0/build/three.module.js";

export default function FlightScene({ state }) {
  const mountRef = useRef(null);

  const sceneRef = useRef();
  const cameraRef = useRef();
  const rendererRef = useRef();
  const planeRef = useRef();

  const stateRef = useRef(state);

  // keep latest state
  useEffect(() => {
    stateRef.current = state;
  }, [state]);

  useEffect(() => {
    // IMPORTANT FIX: Capture the ref value immediately
    const currentMount = mountRef.current;
    if (!currentMount) return;

    // --------------------
    // Scene
    // --------------------
    const scene = new THREE.Scene();
    scene.background = new THREE.Color(0x0b0f14);
    sceneRef.current = scene;

    // Camera
    const camera = new THREE.PerspectiveCamera(
      75,
      currentMount.clientWidth / currentMount.clientHeight,
      0.1,
      1000
    );
    camera.position.set(0, 5, 10);
    cameraRef.current = camera;

    // Renderer
    const renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(
      currentMount.clientWidth,
      currentMount.clientHeight
    );

    currentMount.appendChild(renderer.domElement);
    rendererRef.current = renderer;

    // Lights
    scene.add(new THREE.AmbientLight(0xffffff, 0.4));

    const light = new THREE.DirectionalLight(0xffffff, 1);
    light.position.set(5, 10, 5);
    scene.add(light);

    // Plane
    const plane = new THREE.Mesh(
      new THREE.BoxGeometry(1, 0.3, 2),
      new THREE.MeshStandardMaterial({ color: 0x00ffcc })
    );

    scene.add(plane);
    planeRef.current = plane;

    // Grid
    scene.add(new THREE.GridHelper(200, 50));

    // --------------------
    // Animation loop
    // --------------------
    let animationId;

    const animate = () => {
      animationId = requestAnimationFrame(animate);

      const currentState = stateRef.current;

      // SAFE state usage
      if (currentState && planeRef.current) {
        const p = currentState.position;
        const q = currentState.quaternion;

        if (p) planeRef.current.position.set(p[0], p[1], p[2]);
        if (q) planeRef.current.quaternion.set(q[0], q[1], q[2], q[3]);
      }

      // camera follow
      if (planeRef.current && cameraRef.current) {
        const p = planeRef.current.position;

        cameraRef.current.position.lerp(
          new THREE.Vector3(p.x, p.y + 5, p.z + 12),
          0.08
        );

        cameraRef.current.lookAt(p);
      }

      renderer.render(scene, camera);
    };

    animate();

    // --------------------
    // CLEANUP 
    // --------------------
    return () => {
      cancelAnimationFrame(animationId);

      if (rendererRef.current) {
        rendererRef.current.dispose();
        rendererRef.current.forceContextLoss();
      }

      // IMPORTANT FIX: Use the captured `currentMount` variable
      if (currentMount && rendererRef.current?.domElement) {
        currentMount.removeChild(rendererRef.current.domElement);
      }
    };
  }, []);

 
  return <div ref={mountRef} className="viewport" />;
}