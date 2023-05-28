import React from 'react'
import './App.css'
import { CssVarsProvider, CssBaseline, Box } from '@mui/joy'
import { BottomBar } from './components/bottombar'
import { Outlet } from 'react-router-dom'
import { LoginModal } from './pages/loginmodal'

function App() {
  return (
    <CssVarsProvider>
      <CssBaseline />
      <Box p={3}>
        <Outlet />
      </Box>
      <Box
        sx={{
          position: 'fixed',
          bottom: 0,
          p: 1.5,
          width: '100%',
          display: 'flex',
        }}
      >
        <BottomBar />
      </Box>
      <LoginModal />
    </CssVarsProvider>
  )
}

export default App
