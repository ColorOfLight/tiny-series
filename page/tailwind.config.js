/** @type {import('tailwindcss').Config} */
export default {
  content: [
    "./index.html",
    "./src/**/*.{js,ts,jsx,tsx}",
  ],
  theme: {
    extend: {
      colors: {
        cover: '#00000055'
      },
      spacing: {
        m2: "-0.5rem",
      }
    },
    
  },
  plugins: [],
}
